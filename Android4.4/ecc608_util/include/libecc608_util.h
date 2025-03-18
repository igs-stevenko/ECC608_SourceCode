#ifndef _ECC608_UTIL_H__
#define _ECC608_UTIL_H__

#include "ecc608_ioctl.h"

#define DEV_NAME "/dev/ecc608"

enum {
	DEV_OPEN_FAILED		= -1,
	DEV_OPEN_SUCCESS	= 0,
};

enum {
	UNLOCK				= 0,
	LOCKED				= 1,
};

enum {
	OP_SUCCESS					= 0,		
	OP_FAILED					= -1,
};

void ecc608_dev_close(void);
int ecc608_wakeup(void);
int ecc608_get_devinfo(void);
int ecc608_set_slotconfig(void);
int ecc608_get_config_info(void);
int ecc608_lock_config_zone(void);
int ecc608_write_system_key(unsigned char *key);
int ecc608_get_system_sign(unsigned char *context, unsigned char *signature);
int ecc608_write_game_key(unsigned char *key);
int ecc608_get_game_sign(unsigned char *context, unsigned char *signature);
int ecc608_lock_data_zone(void);
int ecc608_check_lock_status(unsigned char *config_zone, unsigned char *data_zone);
int ecc608_check_slotconfig(void);


#endif
