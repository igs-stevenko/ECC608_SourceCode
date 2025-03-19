#ifndef __VERIFY_H__
#define __VERIFY_H__

#include "openssl/evp.h"
#include "openssl/ec.h"
#include "openssl/bn.h"
#include "openssl/sha.h"
#include "openssl/rand.h"
#include "openssl/ecdsa.h"

#define CURVE_NID           NID_X9_62_prime256v1

#define ECDSA_PUBLIC_X_KEY_SIZE 32
#define ECDSA_PUBLIC_Y_KEY_SIZE 32
#define ECDSA_PUBLIC_KEY_SIZE 64
#define ECDSA_PRIVATEKEY_SIZE 32

#define ECDSA_SIGNATURE_SIZE 64
#define ECDSA_SIGNATURE_RS_SIZE 32


enum {
	OPERATION_SUCCESS = 0,
	OPERATION_FAILED = -1,
	ECC_GENERATE_KEY_FAILED = -2,
	UUID_SIZE_ERROR = -3,
	ECC_SIGN_FAILED = -4,
	ECC_VERIFY_FAILED = -5,
};


int verify(unsigned char *context, unsigned char *in_Signature, unsigned char *pub_key);

#endif
