#ifndef __ECC608_H__
#define __ECC608_H__

/* 設定ecc608 config，並燒斷 */
int set_config(void);

/* 設定game key，放入私鑰 */
int set_gamekey(unsigned char *game_privkey);

/* 燒斷key，一旦燒斷便不可再使用set_gamekey() */
int lock_data_zone(void);

/* 對game key做challenage resopnse，*/
int authentication_game(unsigned char *pubkey);

/* Parm :
 * config_zone : value = 0 -> unlock, value = 1 -> locked
 * data_zone : value = 0 -> unlock, value = 1 -> locked
 *
 * Return :
  * value = 0 -> 操作成功, value < 0 -> 操作失敗 */
int ecc608_check_lock_status(unsigned char *config_zone, unsigned char *data_zone);

/* Return Table */
enum {
	CMD_SUCCESS					= 0,		
	CMD_WAKEUP_FAILED			= -1,
	CMD_CHECK_STATUS_FAILED		= -2,
	CMD_SET_SLOTCONFIG_FAILED	= -3,
	CMD_GET_CONFIG_INFO_FAILED	= -4,
	CMD_LOCK_CONFIG_ZONE_FAILED = -5,
	CMD_WRITE_SYSTEMKEY_FAILED	= -6,
	CMD_WRITE_GAMEKEY_FAILED	= -7,
	CMD_LOCK_DATA_ZONE_FAILED	= -8,
	CMD_CHECK_SLOTCONFIG_FAILED = -9,
	LOCK_STATUS_ERROR			= -10,
	CMD_GET_SYSTEM_SIGN_FAILED	= -11,
	CMD_GET_GAME_SIGN_FAILED	= -12,
	VERIFY_FAILED				= -13,
};

#endif
