#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "verify.h"

EC_KEY *IGS_ECC_GenerateKey_with_PublicKey(int Nid, const unsigned char *Qx, const unsigned char *Qy)
{
	EC_KEY *key = EC_KEY_new();
	EC_GROUP *group = EC_GROUP_new_by_curve_name(Nid);
	EC_POINT *pubkey = EC_POINT_new(group);
	size_t field_size = (EC_GROUP_get_degree(group) + 7) / 8;
	unsigned char *point_buf;

	/* Set the EC_GROUP of a EC_KEY object */
	if (0 == EC_KEY_set_group(key, group))
	{
		EC_KEY_free(key);
		EC_GROUP_free(group);
		EC_POINT_free(pubkey);
		return NULL;
	}

	if (NULL == (point_buf = OPENSSL_malloc(field_size * 2 + 1)))
	{
		EC_KEY_free(key);
		EC_GROUP_free(group);
		EC_POINT_free(pubkey);
		return NULL;
	}

	point_buf[0] = POINT_CONVERSION_UNCOMPRESSED;
	memcpy(&point_buf[1], Qx, field_size);
	memcpy(&point_buf[1 + field_size], Qy, field_size);

	/* Decodes a EC_POINT from a octet string */
	if (0 == EC_POINT_oct2point(group, pubkey, point_buf, (field_size * 2 + 1), NULL))
	{
		EC_KEY_free(key);
		EC_GROUP_free(group);
		EC_POINT_free(pubkey);
		OPENSSL_free(point_buf);
		return NULL;
	}

	/* Set the public key of a EC_KEY object */
	if (0 == EC_KEY_set_public_key(key, pubkey))
	{
		EC_KEY_free(key);
		EC_GROUP_free(group);
		EC_POINT_free(pubkey);
		OPENSSL_free(point_buf);
		return NULL;
	}

err:
	EC_GROUP_free(group);
	EC_POINT_free(pubkey);
	OPENSSL_free(point_buf);
	return key;
}

int IGS_ECC_VerifySignature(unsigned char *Data, size_t DataSize, EC_KEY *Key, unsigned char *R, size_t R_Size, unsigned char *S, size_t S_Size)
{

	ECDSA_SIG *signature = ECDSA_SIG_new();
	BIGNUM *bn_r, *bn_s;
	int ret;

	bn_r = BN_bin2bn(R, R_Size, NULL);
	bn_s = BN_bin2bn(S, S_Size, NULL);
	//ECDSA_SIG_set0(signature, bn_r, bn_s);
	signature->r = bn_r;
	signature->s = bn_s;

	ret = ECDSA_do_verify(Data, 32, signature, Key);

	/*釋放記憶體*/
	ECDSA_SIG_free(signature);
	return ret;
}

/* Verify UUID signature by public key */
int verify(unsigned char *context, unsigned char *in_Signature, unsigned char *pub_key) {
	
	int rtn = 0;
	EC_KEY *ECDSA_Verify_Key = NULL;
	unsigned char ECDSA_Sign_R[ECDSA_SIGNATURE_RS_SIZE] = { 0 };
	unsigned char ECDSA_Sign_S[ECDSA_SIGNATURE_RS_SIZE] = { 0 };

	memcpy(ECDSA_Sign_R, in_Signature, ECDSA_SIGNATURE_RS_SIZE);
	memcpy(ECDSA_Sign_S, &in_Signature[ECDSA_SIGNATURE_RS_SIZE], ECDSA_SIGNATURE_RS_SIZE);

	/* Generate ecdsa key by Public key */
	if (NULL == (ECDSA_Verify_Key = IGS_ECC_GenerateKey_with_PublicKey(CURVE_NID, pub_key, &pub_key[ECDSA_PUBLIC_X_KEY_SIZE])))
	{
		EC_KEY_free(ECDSA_Verify_Key);
		return ECC_GENERATE_KEY_FAILED;
	}

	/* Verity by ecdsa key */
	if (0 == IGS_ECC_VerifySignature(context, 32, ECDSA_Verify_Key, ECDSA_Sign_R, sizeof(ECDSA_Sign_R), ECDSA_Sign_S, sizeof(ECDSA_Sign_S)))
	{
		EC_KEY_free(ECDSA_Verify_Key);
		return ECC_VERIFY_FAILED;
	}

	EC_KEY_free(ECDSA_Verify_Key);
	return OPERATION_SUCCESS;
}


