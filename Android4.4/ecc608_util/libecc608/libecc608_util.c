#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "../include/libecc608_util.h"

int g_fd = -1;

#define RETRY_COUNTS	10
#define WAIT_TIME 500*1000

static int ecc608_dev_open(void){

	if(g_fd <= 0){

		g_fd = open(DEV_NAME, O_RDWR);
		if(g_fd <= 0){
			return DEV_OPEN_FAILED;
		}
	}

	return DEV_OPEN_SUCCESS;
}

void ecc608_dev_close(void){

	if(g_fd > 0)	close(g_fd);
	g_fd = -1;
}


int ecc608_wakeup(void){

	int rtn = 0;
	int i;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}

	/* Retry 5 times */
	for(i=0; i<RETRY_COUNTS; i++){
		
		rtn = ioctl(g_fd, ECC608_ENABLE, NULL);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else
	  return OP_SUCCESS;
}

int ecc608_get_devinfo(void){

	int rtn = 0;
	int i;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}
	
	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_GETDEVINFO, NULL);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_set_slotconfig(void){

	int rtn = 0;
	int i;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}
	
	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_SET_SLOTCONFIG, NULL);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_get_config_info(void){

	int rtn = 0;
	int i=0;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}

	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_GET_CONFIG_INFO, NULL);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_lock_config_zone(void){
	
	int rtn = 0;
	int i;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}

	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_LOCK_CONFIG_ZONE, NULL);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_write_system_key(unsigned char *key){

	int rtn = 0;
	int i;
	struct ecc608_info info;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}

	memset(&info, 0, sizeof(struct ecc608_info));
	memcpy(info.system_key, key, 32);

	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_WRITE_SYSTEM_KEY, &info);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}


	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_get_system_sign(unsigned char *context, unsigned char *signature){

	int rtn = 0;
	int i;
	struct ecc608_info info;
	
	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}
	
	memset(&info, 0, sizeof(struct ecc608_info));
	
	memcpy(info.system_context, context, 32);
	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_GET_SYSTEM_SIGNATURE, &info);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	memcpy(signature, info.system_sign, 64);
	
	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;

}

int ecc608_write_game_key(unsigned char *key){

	int rtn = 0;
	int i;
	struct ecc608_info info;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}
	
	memset(&info, 0, sizeof(struct ecc608_info));
	memcpy(info.game_key, key, 32);

	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_WRITE_GAME_KEY, &info);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_get_game_sign(unsigned char *context, unsigned char *signature){

	int rtn = 0;
	int i;
	struct ecc608_info info;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}
	
	memset(&info, 0, sizeof(struct ecc608_info));
	memcpy(info.game_context, context, 32);
	
	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_GET_GAME_SIGNATURE, &info);
		if(rtn == OP_SUCCESS)
		  break;
		
		usleep(WAIT_TIME);
	}

	memcpy(signature, info.game_sign, 64);
	
	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_lock_data_zone(void){

	int rtn = 0;
	int i;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}
	
	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_LOCK_OTP_DATA, NULL);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_check_lock_status(unsigned char *config_zone, unsigned char *data_zone){
	
	int rtn = 0;
	int i;
	struct ecc608_info info;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}
		
	memset(&info, 0, sizeof(struct ecc608_info));
	
	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_CHECK_LOCK_STATUS, &info);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(info.config_zone_locked == 0x01)
		*config_zone = LOCKED;
	else
		*config_zone = UNLOCK;

	
	if(info.data_zone_locked == 0x01)
		*data_zone = LOCKED;
	else
		*data_zone = UNLOCK;

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}

int ecc608_check_slotconfig(void){

	int rtn = 0;
	int i;

	if(ecc608_dev_open() == DEV_OPEN_FAILED){
		return DEV_OPEN_FAILED;
	}
	
	for(i=0; i<RETRY_COUNTS; i++){
		rtn = ioctl(g_fd, ECC608_CHECK_SLOTCONFIG, NULL);
		if(rtn == OP_SUCCESS)
		  break;

		usleep(WAIT_TIME);
	}

	if(i==RETRY_COUNTS)	
	  return OP_FAILED;
	else		
	  return OP_SUCCESS;
}
