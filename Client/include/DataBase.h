#ifndef CLIENT_INCLUDE_DATABASE_H_
#define CLIENT_INCLUDE_DATABASE_H_

#define ANDLINK_CONF_FILE     "/etc/config/fhandlink"
#define ANDLINK_REG_FILE      "/etc/config/andlinkreginfo"
#define GW_VERSION            "/etc/fh_version"
#define LINE_LEN              256
#define REALTEK_DEFAULT_IP    "192.168.1.254"

typedef struct
{
    char radio[16];                 /**  mac address of the host */
    char ssidname[64];              /**  ssid name */
    char securitymode[32];          /**  security */
    char password[64];              /**  password */
    int index;                      /**  ssid index */
    int enable;                     /**  ssid enable */
    int maxassociatenum;            /**  max associate num*/
    int ssidadenable;               /**  SSID Advertisement Enabled */
}wifi_conf_info;

#ifdef __cplusplus
extern "C"
{
#endif

void format_mac_from_colon(char *mac, const char *src);
void riprt(char *str);
int setcfgx(const char *filpath, const char *nam, char *val);
int getcfgx(const char *filpath, const char *nam, char *val);

/*
 * Function declaration
 */
int Client_database_init();
void save_wlan_conn_flag(int wlan_conn_flag);
int get_wlan_conn_flag(void);
void save_gatewayip(char *gwip);
void save_deviceip(char *devip);
void get_deviceip(char *devip);
int check_devip(char *ip_address);
void check_andlink_configuration_file();
void save_device_info(char *product_class, char *hardwareversion, char *softwareversion, char *devicemac);
void get_deviceversion(char *product_class, char *hardwareversion, char *softwareversion);
void get_devicetype(char *devtype);
void get_deviceid(char *devid);
void get_producttoken(char *devproducttoken);
void get_devicetoken(char *devtoken);
void get_gwtoken(char *gwtoken);
void get_devicemac(char *devmac);
void get_devicesn(char *devsn);
void get_device_wanmac(char *devwanmac);
void get_deviceencrypt(char *devencrypt);
void set_deviceencrypt(char *devencrypt);
void get_device_wanip(char *ip_address);
void get_device_wanbcastip(char *ip_address);
int is_childdev_conf_exist(char *childdeviceid);
void add_childdev_conf(char *childdeviceid);
void get_childdev_paramvalue(char *childdeviceid, char *paramname, char *paramvalue);
void set_childdev_paramvalue(char *childdeviceid, char *paramname, char *paramvalue);
void del_childdev_conf(char *childdeviceid);
int check_andlink(char *device_type, char *andlink_token);
void save_smartnet_reg_info(char *device_id, char *dev_token, char *gw_token, char *heartbeat_time);
void save_smartnet_synccode_info(char *synccode);
void save_smartnet_macfilter_info(int macfilterenable, int macfilterpolicy, int macnum, char * *macentries);
int get_smartnet_synccode_info(void);
void get_smartnet_wifi24gconf_info(int *enable, char *transmitpower, int *channel);
void get_smartnet_wifi5gconf_info(int *enable, char *transmitpower, int *channel);
void get_smartnet_wifi24gssidconf_info(wifi_conf_info *wifissidconf);
void get_smartnet_wifi5gssidconf_info(wifi_conf_info *wifissidconf);
void get_smartnet_uplink_info(char *uplinktype, char *rxrate, char *txrate);

/*
 * The following function is implemented by myself
 */
int  get_led_status();
void get_roaming_status(int* p_enable, int* p_lowRssi24G, int* p_lowRssi5G);
void get_macfilter_status(int* p_enable, int* p_policy, char* p_entryBuffer, int p_bufferSize);
int  get_radio_number();
void get_radio_status(int index, char* p_radioName, int* p_radioEnable, char* p_radioPower, int* p_radioChannel);
int  get_radio_configuration_number();
void get_radio_configuration_status(
	int p_index, char* p_configName, int* p_configEnable, char* p_configSSID, char* p_configSecurityMode,
	char* p_configPwd, int* p_configMaxAssociateNum, int* p_configSSIDAdvertisementEnabled);

#ifdef __cplusplus
}
#endif

#endif /* CLIENT_INCLUDE_DATABASE_H_ */
