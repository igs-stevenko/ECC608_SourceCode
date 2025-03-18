#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>

#include "libecc608_api.h"
#include "libecc608_util.h"
#include "verify.h"

int _set_config(void){

	int rtn = 0;
	unsigned char config_zone_status;
	unsigned char data_zone_status;

	unsigned char system_key[32] = {
        0x25, 0xFB, 0x89, 0xD3, 0xC5, 0x05, 0x7D, 0xB6,
        0x9C, 0x27, 0x23, 0xDA, 0xF0, 0x07, 0x48, 0x22,
        0x9D, 0x6B, 0x8F, 0x31, 0xA9, 0xED, 0x18, 0x17,
        0x62, 0x26, 0x45, 0x3A, 0x00, 0xB0, 0x73, 0xE5
    };


	rtn = ecc608_wakeup();
	if(rtn != CMD_SUCCESS){
		return CMD_WAKEUP_FAILED;
	}

	rtn = ecc608_check_lock_status(&config_zone_status, &data_zone_status);
	if(rtn != CMD_SUCCESS){
		return CMD_CHECK_STATUS_FAILED;
	}

	/* Data zone locked, return failed */
	if(data_zone_status == LOCKED){
		return LOCK_STATUS_ERROR;
	}

	/* Config zone unlock, setting config zone */
	if(config_zone_status == UNLOCK){

		rtn = ecc608_set_slotconfig();
		if(rtn != CMD_SUCCESS){
			return CMD_SET_SLOTCONFIG_FAILED; 
		}

		rtn = ecc608_get_config_info();
		if(rtn != CMD_SUCCESS){
			return CMD_GET_CONFIG_INFO_FAILED;
		}

		rtn = ecc608_lock_config_zone();
		if(rtn != CMD_SUCCESS){
			return CMD_LOCK_CONFIG_ZONE_FAILED;
		}

		rtn = ecc608_check_lock_status(&config_zone_status, &data_zone_status);
		if(rtn != CMD_SUCCESS){
			return CMD_CHECK_STATUS_FAILED;
		}

		if(config_zone_status == UNLOCK){
			return LOCK_STATUS_ERROR;
		}
	}
	else{
		/* Check config zone value is correct. */
		rtn = ecc608_check_slotconfig();
		if(rtn != CMD_SUCCESS){
			return CMD_CHECK_SLOTCONFIG_FAILED;	
		}
	}

	rtn = ecc608_write_system_key(system_key);
	if(rtn != CMD_SUCCESS){
		return CMD_WRITE_SYSTEMKEY_FAILED;
	}

	return CMD_SUCCESS;
}

int _set_gamekey(unsigned char *game_key){

	int rtn = 0;
	unsigned char config_zone_status;
	unsigned char data_zone_status;

	rtn = ecc608_wakeup();
	if(rtn != CMD_SUCCESS){
		return CMD_WAKEUP_FAILED;
	}

	rtn = ecc608_check_lock_status(&config_zone_status, &data_zone_status);
	if(rtn != CMD_SUCCESS){
		return CMD_CHECK_STATUS_FAILED;
	}

	if((config_zone_status == UNLOCK) || (data_zone_status == LOCKED)){
		return LOCK_STATUS_ERROR;
	}

	rtn = ecc608_write_game_key(game_key);
	if(rtn != CMD_SUCCESS){
		return CMD_WRITE_GAMEKEY_FAILED;
	}

	return CMD_SUCCESS;
}

int _lock_data_zone(void){

	int rtn = 0;
	unsigned char config_zone_status;
	unsigned char data_zone_status;

	rtn = ecc608_check_lock_status(&config_zone_status, &data_zone_status);
	if(rtn != CMD_SUCCESS){
		return CMD_CHECK_STATUS_FAILED;
	}

	if((config_zone_status == UNLOCK) || (data_zone_status == LOCKED)){
		return LOCK_STATUS_ERROR;
	}
	
	rtn = ecc608_lock_data_zone();
	if(rtn != CMD_SUCCESS){
		return CMD_LOCK_DATA_ZONE_FAILED;
	}
	
	rtn = ecc608_check_lock_status(&config_zone_status, &data_zone_status);
	if(rtn != CMD_SUCCESS){
		return CMD_CHECK_STATUS_FAILED;
	}


	if((config_zone_status == UNLOCK) || (data_zone_status == UNLOCK)){
		return LOCK_STATUS_ERROR;
	}

	return CMD_SUCCESS;

}


int _authentication_system(unsigned char *pubkey){

	int rtn = 0;
	int i;
	unsigned char context[32] = {0x00};
	unsigned char signature[64] = {0x00};

	srand(time(NULL));

	for(i=0; i<32; i++){
		context[i] = (rand() % 256);
	}

	rtn = ecc608_wakeup();
	if(rtn != CMD_SUCCESS){
		return CMD_WAKEUP_FAILED;
	}

	rtn = ecc608_get_system_sign(context, signature);
	if(rtn != CMD_SUCCESS)  return CMD_GET_SYSTEM_SIGN_FAILED;

	rtn = verify(context, signature, pubkey);
	if(rtn != OPERATION_SUCCESS){	
		return VERIFY_FAILED;	
	}
	
	return CMD_SUCCESS;
}

int _authentication_game(unsigned char *pubkey){

	int rtn = 0;
	int i;
	unsigned char context[32] = {0x00};
	unsigned char signature[64] = {0x00};

	printf("### [%s][%d] ###\n", __func__, __LINE__); 

	srand(time(NULL));

	for(i=0; i<32; i++){
		context[i] = (rand() % 256);
	}

	rtn = ecc608_wakeup();
	if(rtn != CMD_SUCCESS){
		return CMD_WAKEUP_FAILED;
	}

	rtn = ecc608_get_game_sign(context, signature);
	if(rtn != CMD_SUCCESS)  return CMD_GET_GAME_SIGN_FAILED;
	
	rtn = verify(context, signature, pubkey);
	if(rtn != OPERATION_SUCCESS)	return VERIFY_FAILED;	
	
	return CMD_SUCCESS;
}


int set_config(void){

	return _set_config();
}

int set_gamekey(unsigned char *game_privkey){

	int rtn = 0;
	
	rtn = _set_gamekey(game_privkey);

	return rtn;
}

int lock_data_zone(void){

	return _lock_data_zone();
}

int authentication_system(unsigned char *pubkey){

	return _authentication_system(pubkey);
}

int authentication_game(unsigned char *pubkey){

	return _authentication_game(pubkey);
}

