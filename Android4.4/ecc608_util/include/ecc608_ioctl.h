#ifndef __ECC608_IOCTL_H__
#define __ECC608_IOCTL_H__

#define MAGIC_NUM       	0x12

#define ECC608_GETDEVINFO					_IO(MAGIC_NUM, 0)
#define ECC608_ENABLE						_IO(MAGIC_NUM, 1)
#define ECC608_SET_SLOTCONFIG				_IO(MAGIC_NUM, 2)
#define ECC608_GET_CONFIG_INFO				_IO(MAGIC_NUM, 3)
#define ECC608_LOCK_CONFIG_ZONE				_IO(MAGIC_NUM, 4)

#define ECC608_WRITE_SYSTEM_KEY				_IOR(MAGIC_NUM, 5, struct ecc608_info)
#define ECC608_GET_SYSTEM_SIGNATURE			_IOWR(MAGIC_NUM, 6, struct ecc608_info)

#define ECC608_WRITE_GAME_KEY				_IOR(MAGIC_NUM, 7, struct ecc608_info)
#define ECC608_GET_GAME_SIGNATURE			_IOWR(MAGIC_NUM, 8, struct ecc608_info)

#define ECC608_LOCK_OTP_DATA				_IO(MAGIC_NUM, 9)

#define ECC608_CHECK_LOCK_STATUS			_IOWR(MAGIC_NUM, 10, struct ecc608_info)

#define ECC608_CHECK_SLOTCONFIG				_IO(MAGIC_NUM, 11)

struct ecc608_info {

	unsigned char system_context[32];
	unsigned char system_key[32];
	unsigned char system_sign[64];
	
	unsigned char game_context[32];
	unsigned char game_key[32];
	unsigned char game_sign[64];

	unsigned char config_zone_locked;
	unsigned char data_zone_locked;

};

#endif

