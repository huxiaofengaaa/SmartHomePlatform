#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "DataBase.h"

#ifdef CROSS_BUILD
#include "apmib.h"
#include "rtl_link.h"
#include "rtl_link_utils.h"
#include "rtl_link_list.h"

//static pthread_mutex_t g_andlink_data_mutex = PTHREAD_MUTEX_INITIALIZER;
//
//static void get_andlink_database(char *section, char *option, char *optval)
//{
//	pthread_mutex_lock(&g_andlink_data_mutex);
//	getcfgx(ANDLINK_CONF_FILE, option, optval);
//	pthread_mutex_unlock(&g_andlink_data_mutex);
//}
//
//static void set_andlink_database(char *section, char *option, char *optval)
//{
//	pthread_mutex_lock(&g_andlink_data_mutex);
//	setcfgx(ANDLINK_CONF_FILE, option, optval);
//	pthread_mutex_unlock(&g_andlink_data_mutex);
//}

#define MAX_LINES 1024
#define LINE_LEN  256
#define NAME_LEN  128
#define LINE(x)   file.lines[x].line
#define LINE_CNT  file.line_cnt
#define ZEROIT(a)   memset(a, 0, sizeof(a))

struct one_line
{
	char line[LINE_LEN];
	int char_cnt;
};

struct file_content
{
	struct one_line lines[MAX_LINES];
	int line_cnt;
};

void format_mac_from_colon(char *mac, const char *src)
{
	int index = 0;
	int num = 0;
	int len = strlen(src);
	int l_numberBeforeSemicolon = 0;
	if (NULL == src || len == 0 || NULL == mac)
	{
		return;
	}

	for (index = 0; index < len; index++)
	{
		if (':' != src[index])
		{
			if(l_numberBeforeSemicolon >= 2)
			{
				mac[num] = ':';
				num++;
				l_numberBeforeSemicolon = 0;
			}
			mac[num] = src[index];
			if (src[index] <= 'f' && src[index] >= 'a')
			{
				mac[num] -= 32;
			}
			l_numberBeforeSemicolon += 1;
			num++;
		}
		else
		{
			l_numberBeforeSemicolon = 0;
		}
	}
	mac[num] = '\0';
	return;
}

void riprt(char *str)
{
	int len, i;
	if (str == NULL)	return;
	len = strlen(str);
	if (len == 0) return;
	for (i = 0; i < len; i++)
	{
		if (str[i] == '\r' || str[i] == '\n')
		{
			str[i] = '\0';
		}
	}
}

int setcfgx(const char *filpath, const char *nam, char *val)
{
	FILE *fd = NULL;
	struct file_content file;
	char tmp[LINE_LEN];
	char *saveptr1, *saveptr2, *p = NULL;
	int i = 0, ret = 0;

	if (filpath == NULL || nam == NULL || val == NULL)
	{
		return ret;
	}

	fd = fopen(filpath, "r");
	if (fd == NULL)
	{
		return ret;
	}
	memset(&file, 0, sizeof(file));

	//read file stream to memory
	while (fgets(LINE(LINE_CNT), LINE_LEN, fd) != NULL)
	{
		LINE_CNT++;
	}

	//modify memory
	for (i = 0; i < LINE_CNT; i++)
	{
		strcpy(tmp, LINE(i));
		if (tmp[0] == '#')continue;
		if ((p = strtok_r(tmp, "=", &saveptr1)) != NULL)
		{
			if (!strcmp(p, nam))
			{
				sprintf(LINE(i), "%s=%s\n", nam, val);
				ret = 1;
				break;
			}
		}
		if ((p = strtok_r(tmp, " ", &saveptr2)) != NULL)
		{
			if (!strcmp(p, nam))
			{
				sprintf(LINE(i), "%s %s\n", nam, val);
				ret = 1;
				break;
			}
		}
	}

	fclose(fd);
	if(i == LINE_CNT)
	{
		sprintf(LINE(i), "%s=%s\n", nam, val);
		LINE_CNT++;
	}
	//write back to file
	fd = fopen(filpath, "w");
	if (fd == NULL)
	{
		return 0;
	}
	for (i = 0; i < LINE_CNT; i++)
	{
		if (fputs(LINE(i), fd) == EOF)
		{
			break;
		}
	}

	fflush(fd);
	fclose(fd);
	return ret;
}

int getcfgx(const char *filpath, const char *nam, char *val)
{
	FILE *fd = NULL;
	struct file_content file;
	char *saveptr1, *saveptr2, *p = NULL;
	int i = 0, ret = 0;

	if (filpath == NULL || nam == NULL || val == NULL)
	{
		return ret;
	}

	fd = fopen(filpath, "r");
	if (fd == NULL)
	{
		return ret;
	}

	memset(&file, 0, sizeof(file));
	//read file stream to memory
	while (fgets(LINE(LINE_CNT), LINE_LEN, fd) != NULL)
	{
		LINE_CNT++;
	}

	//compare and put out the value
	for (i = 0; i < LINE_CNT; i++)
	{
		if (LINE(i)[0] == '#')continue;
		if ((p = strtok_r(LINE(i), "=", &saveptr1)) != NULL)
		{
			if (!strcmp(p, nam))
			{
				p = strtok_r(NULL, "=", &saveptr1);
				strcpy(val, p);
				riprt(val);
				ret = 1;
				break;
			}
		}
		if ((p = strtok_r(LINE(i), " ", &saveptr2)) != NULL)
		{
			if (!strcmp(p, nam))
			{
				p = strtok_r(NULL, " ", &saveptr2);
				strcpy(val, p);
				riprt(val);
				ret = 1;
				break;
			}
		}
	}
	fclose(fd);
	return ret;
}

int Client_database_init()
{
	int pid_fd;
	pid_fd = pidfile_acquire(RTL_LINK_PID_FILE);
	if (pid_fd < 0)
	{
		return -1;
	}
	pidfile_write_release(pid_fd);

	if (!apmib_init())
	{
		return -1;
	}
	printf("Client_database_init success\n");
	return 0;
}

void save_wlan_conn_flag(int wlan_conn_flag)
{
	apmib_set(MIB_FH_LINK_WLANCONNECTED, (void *)&wlan_conn_flag);
	apmib_update(CURRENT_SETTING);
}

int get_wlan_conn_flag(void)
{
	int wlan_conn_flag = 0;
	apmib_get(MIB_FH_LINK_WLANCONNECTED, (void *)&wlan_conn_flag);
	return wlan_conn_flag;
}

void save_gatewayip(char *gwip)
{
	if(gwip != NULL)
	{
		struct in_addr ip;
		inet_aton(gwip, &ip);
		apmib_set(MIB_RTL_LINK_GW_IP_ADDR, (void *)&ip);
		apmib_update(CURRENT_SETTING);
	}
}

void save_deviceip(char *devip)
{
	setcfgx(ANDLINK_CONF_FILE, "ipAddress", devip);
}

void get_deviceip(char *devip)
{
	if(devip != NULL)
	{
		getcfgx(ANDLINK_CONF_FILE, "ipAddress", devip);
	}
}

int check_devip(char *ip_address)
{
	char gwip[32] = "";
	int ret = 0;

	ret = rtl_link_getGatewayIp(gwip);
	if (0 != ret)
	{
		return -1;
	}

	if (NULL == ip_address)
	{
		return -1;
	}

	if (0 == strlen(ip_address))
	{
		return -1;
	}

	if (0 == strcmp(REALTEK_DEFAULT_IP, ip_address))
	{
		return -1;
	}

	return 0;
}

void check_andlink_configuration_file()
{
	if(access(ANDLINK_CONF_FILE, 0) < 0)
	{
		char l_cmd[64] = { 0 };
		snprintf(l_cmd, sizeof(l_cmd), "touch %s", ANDLINK_CONF_FILE);
		system(l_cmd);
		printf("check_andlink_configuration_file - create file %s\n", ANDLINK_CONF_FILE);
#if defined(ANDLINK_PRODUCT_TYPE) && (ANDLINK_PRODUCT_TYPE == 0 || ANDLINK_PRODUCT_TYPE == 2)
		setcfgx(ANDLINK_CONF_FILE, "SyncCode", "0");
#endif
	}
}

void save_device_info(char *product_class, char *hardwareversion, char *softwareversion, char *devicemac)
{
#if defined(ANDLINK_PRODUCT_TYPE) && (ANDLINK_PRODUCT_TYPE == 0 || ANDLINK_PRODUCT_TYPE == 2)
	setcfgx(ANDLINK_CONF_FILE, "model", product_class);
	setcfgx(ANDLINK_CONF_FILE, "factoryName", "fiberhome");
#endif
	setcfgx(ANDLINK_CONF_FILE, "softwareVersion", hardwareversion);
	setcfgx(ANDLINK_CONF_FILE, "softwareVersion", softwareversion);
	setcfgx(ANDLINK_CONF_FILE, "deviceMac", devicemac);
}

void get_deviceversion(char *product_class, char *hardwareversion, char *softwareversion)
{
	if (NULL == product_class || NULL == hardwareversion || NULL == softwareversion)
	{
		return;
	}

	getcfgx(GW_VERSION, "sw_version", softwareversion);
	getcfgx(GW_VERSION, "hw_version", hardwareversion);
	getcfgx(GW_VERSION, "model", product_class);
}

void get_devicetype(char *devtype)
{
	if(devtype != NULL)
	{
		apmib_get(MIB_RTL_LINK_DEVTYPE, (void *)devtype);
	}
}

void get_deviceid(char *devid)
{
	if (NULL != devid)
	{
		apmib_get(MIB_RTL_LINK_DEVID, (void *)devid);
	}
}

void get_producttoken(char *devproducttoken)
{
	if (NULL != devproducttoken)
	{
		apmib_get(MIB_RTL_LINK_PRODTOKEN, (void *)devproducttoken);
	}
}

void get_devicetoken(char *devtoken)
{
	if (NULL != devtoken)
	{
		apmib_get(MIB_RTL_LINK_DEVTOKEN, (void *)devtoken);
	}
}

void get_gwtoken(char *gwtoken)
{
	if (NULL != gwtoken)
	{
		apmib_get(MIB_RTL_LINK_GWTOKEN, (void *)gwtoken);
	}
}

void get_devicemac(char *devmac)
{
	if (NULL == devmac)
	{
		char mac[64];
		get_device_wanmac(mac);
		format_mac_from_colon(devmac, mac);
	}
}

void get_devicesn(char *devsn)
{
	if (NULL != devsn)
	{
		get_device_wanmac(devsn);
		apmib_get(MIB_RTL_LINK_SN, (void *)devsn);
	}
}

void get_device_wanmac(char *devwanmac)
{
	if (NULL != devwanmac)
	{
		rtl_link_getWanMacAddr(devwanmac);
	}
}

void get_deviceencrypt(char *devencrypt)
{
	if (NULL != devencrypt)
	{
		int encryptvalue = 0;
		apmib_get(MIB_RTL_LINK_ENCRYPT, (void *)&encryptvalue);
		sprintf(devencrypt, "%d", encryptvalue);
	}
}

void set_deviceencrypt(char *devencrypt)
{
	if (NULL != devencrypt)
	{
		int encryptvalue = atoi(devencrypt);
		apmib_set(MIB_RTL_LINK_ENCRYPT, (void *)&encryptvalue);
	}
}

void get_device_wanip(char *ip_address)
{
	if (NULL != ip_address)
	{
		struct in_addr ipaddr;
		if (getInAddr(BR_IF, IP_ADDR_T, (void *)&ipaddr))
		{
			inet_ntop(AF_INET, &ipaddr, ip_address, INET_ADDRSTRLEN);
		}
	}
}

void get_device_wanbcastip(char *ip_address)
{
	if (NULL != ip_address)
	{
		struct in_addr ipaddr;
		struct in_addr netmask;
		if (getInAddr(BR_IF, IP_ADDR_T, (void *)&ipaddr)
				&& getInAddr(BR_IF, NET_MASK_T, (void *)&netmask))
		{
			ipaddr.s_addr = ipaddr.s_addr | (~netmask.s_addr);
			inet_ntop(AF_INET, &ipaddr, ip_address, INET_ADDRSTRLEN);
		}
	}
}

int is_childdev_conf_exist(char *childdeviceid)
{
	int ret = 0;
	if (NULL == childdeviceid)
	{

	}
	return ret;
}

void add_childdev_conf(char *childdeviceid)
{
	if (NULL == childdeviceid)
	{

	}
}

void get_childdev_paramvalue(char *childdeviceid, char *paramname, char *paramvalue)
{
	if (NULL == childdeviceid || NULL == paramname || NULL == paramvalue)
	{

	}
}

void set_childdev_paramvalue(char *childdeviceid, char *paramname, char *paramvalue)
{
	if (NULL == childdeviceid || NULL == paramname || NULL == paramvalue)
	{

	}
}

void del_childdev_conf(char *childdeviceid)
{
	if (NULL == childdeviceid)
	{

	}
}

int check_andlink(char *device_type, char *andlink_token)
{
	char andlinktokentmp[LINE_LEN];
	char optname[64];

	ZEROIT(andlinktokentmp);
	ZEROIT(optname);
	sprintf(optname, "%s_andlinkToken", device_type);
	getcfgx(ANDLINK_REG_FILE, optname, andlinktokentmp);
	apmib_get(MIB_RTL_LINK_ANDLINKTOKEN, (void *)andlinktokentmp);

	//latest spec will not check andlinktoken!!!!!!!!!!!!!!!!!!!!  2017-10-26 zhujunjie
	return 0;
}

void save_smartnet_reg_info(char *device_id, char *dev_token, char *gw_token, char *heartbeat_time)
{
	unsigned long hbtime = 0;

	if (NULL != device_id)
	{
		apmib_set(MIB_RTL_LINK_DEVID, (void *)device_id);
	}
	if (NULL != dev_token)
	{
		apmib_set(MIB_RTL_LINK_DEVTOKEN, (void *)dev_token);
	}
	if (NULL != heartbeat_time)
	{
		hbtime = atoi(heartbeat_time);
		hbtime = hbtime ? hbtime : 30;
		apmib_set(MIB_RTL_LINK_HBTIME, (void *)&hbtime);
	}
	if (NULL != gw_token)
	{
		apmib_set(MIB_RTL_LINK_GWTOKEN, (void *)gw_token);
	}
	apmib_update(CURRENT_SETTING);
}

void save_smartnet_synccode_info(char *synccode)
{
	apmib_set(MIB_RTL_LINK_SYNCCODE, (void *)synccode);
	apmib_update(CURRENT_SETTING);
}

void save_smartnet_macfilter_info(int macfilterenable, int macfilterpolicy, int macnum, char * *macentries)
{

	return;
}

int get_smartnet_synccode_info(void)
{
	int synccode = 0;
	char paramvalue[64] = {0};
	apmib_get(MIB_RTL_LINK_SYNCCODE, (void *)paramvalue);
	synccode = atoi(paramvalue);
	return synccode;
}

void get_smartnet_wifi24gconf_info(int *enable, char *transmitpower, int *channel)
{
	char paramvalue[64] = {0};
	int synccode = 0;
	RfStatus rfstatus;
	int i = 0;

	rtl_link_getRFInfo(&rfstatus);
	for (i = 0; i < rfstatus.num; i++)
	{
		if (PHYBAND_2G == rfstatus.rf[i].band)
		{
			*enable = rfstatus.rf[i].enable;
			sprintf(transmitpower, "%d", rfstatus.rf[i].power);
			*channel = rfstatus.rf[i].channel;
		}
	}
}

void get_smartnet_wifi5gconf_info(int *enable, char *transmitpower, int *channel)
{
	char paramvalue[64] = {0};
	int synccode = 0;
	RfStatus rfstatus;
	int i = 0;

	rtl_link_getRFInfo(&rfstatus);
	for (i = 0; i < rfstatus.num; i++)
	{
		if (PHYBAND_5G == rfstatus.rf[i].band)
		{
			*enable = rfstatus.rf[i].enable;
			sprintf(transmitpower, "%d", rfstatus.rf[i].power);
			*channel = rfstatus.rf[i].channel;
		}
	}
}

void get_smartnet_wifi24gssidconf_info(wifi_conf_info *wifissidconf)
{
	char paramvalue[64] = {0};
	int synccode = 0;
	int index = 0;

	WiFiConfig wificonf;
	int i = 0;

	rtl_link_getWiFiConfig(&wificonf);
	for (i = 0; i < wificonf.num; i++)
	{
		if (PHYBAND_2G == wificonf.ap[i].band)
		{
			wifissidconf->index = wificonf.ap[i].idx;
			wifissidconf->enable = wificonf.ap[i].enable;
			strcpy(wifissidconf->ssidname, wificonf.ap[i].ssid);
			if (wificonf.ap[i].encrypt == ENCRYPT_DISABLED)
			{
				strcpy(wifissidconf->securitymode, "None");
				strcpy(wifissidconf->password, "");
			}
			else if (wificonf.ap[i].encrypt == ENCRYPT_WEP)
			{
				if (wificonf.ap[i].wep_type == WEP64)
				{
					strcpy(wifissidconf->securitymode, "WEP-64");
					strcpy(wifissidconf->password, wificonf.ap[i].key);
				}
				else if (wificonf.ap[i].wep_type == WEP128)
				{
					strcpy(wifissidconf->securitymode, "WEP-128");
					strcpy(wifissidconf->password, wificonf.ap[i].key);
				}
			}
			else if (wificonf.ap[i].encrypt == ENCRYPT_WPA)
			{
				strcpy(wifissidconf->securitymode, "WPA-Personal");
				strcpy(wifissidconf->password, wificonf.ap[i].key);
			}
			else if (wificonf.ap[i].encrypt == ENCRYPT_WPA2)
			{
				strcpy(wifissidconf->securitymode, "WPA2-Personal");
				strcpy(wifissidconf->password, wificonf.ap[i].key);
			}
			else if (wificonf.ap[i].encrypt == ENCRYPT_WPA2_MIXED)
			{
				strcpy(wifissidconf->securitymode, "MIXED-WPAPSK2");
				strcpy(wifissidconf->password, wificonf.ap[i].key);
			}
			wifissidconf->maxassociatenum = wificonf.ap[i].max_sta_num;
			wifissidconf->ssidadenable = wificonf.ap[i].broadcast_ssid;
			break;
		}
	}
}

void get_smartnet_wifi5gssidconf_info(wifi_conf_info *wifissidconf)
{
	char paramvalue[64] = {0};
	int synccode = 0;
	int index = 0;
	WiFiConfig wificonf;
	int i = 0;

	rtl_link_getWiFiConfig(&wificonf);
	for (i = 0; i < wificonf.num; i++)
	{
		if (PHYBAND_5G == wificonf.ap[i].band)
		{
			wifissidconf->index = wificonf.ap[i].idx;
			wifissidconf->enable = wificonf.ap[i].enable;
			strcpy(wifissidconf->ssidname, wificonf.ap[i].ssid);
			if (wificonf.ap[i].encrypt == ENCRYPT_DISABLED)
			{
				strcpy(wifissidconf->securitymode, "None");
				strcpy(wifissidconf->password, "");
			}
			else if (wificonf.ap[i].encrypt == ENCRYPT_WEP)
			{
				if (wificonf.ap[i].wep_type == WEP64)
				{
					strcpy(wifissidconf->securitymode, "WEP-64");
					strcpy(wifissidconf->password, wificonf.ap[i].key);
				}
				else if (wificonf.ap[i].wep_type == WEP128)
				{
					strcpy(wifissidconf->securitymode, "WEP-128");
					strcpy(wifissidconf->password, wificonf.ap[i].key);
				}
			}
			else if (wificonf.ap[i].encrypt == ENCRYPT_WPA)
			{
				strcpy(wifissidconf->securitymode, "WPA-Personal");
				strcpy(wifissidconf->password, wificonf.ap[i].key);
			}
			else if (wificonf.ap[i].encrypt == ENCRYPT_WPA2)
			{
				strcpy(wifissidconf->securitymode, "WPA2-Personal");
				strcpy(wifissidconf->password, wificonf.ap[i].key);
			}
			else if (wificonf.ap[i].encrypt == ENCRYPT_WPA2_MIXED)
			{
				strcpy(wifissidconf->securitymode, "MIXED-WPAPSK2");
				strcpy(wifissidconf->password, wificonf.ap[i].key);
			}
			wifissidconf->maxassociatenum = wificonf.ap[i].max_sta_num;
			wifissidconf->ssidadenable = wificonf.ap[i].broadcast_ssid;

			break;
		}
	}
}

void get_smartnet_uplink_info(char *uplinktype, char *rxrate, char *txrate)
{
	char paramvalue[64] = {0};
	int synccode = 0;

	return;
}


////////////////////////////////////////////////////////////////////
/*
 * The following function is implemented by myself
 */

int get_led_status()
{
	LEDStatus led;
    memset(&led, 0, sizeof(led));
    rtl_link_getLEDStatus(&led);
    return led.on;
}

void get_roaming_status(int* p_enable, int* p_lowRssi24G, int* p_lowRssi5G)
{
	RoamingConfig roaming;
    memset(&roaming, 0, sizeof(roaming));
    rtl_link_getRoamingStatus(&roaming);
    if(p_enable != NULL)
    {
    	*p_enable = roaming.enable;
    }
    if(p_lowRssi24G != NULL)
    {
    	*p_lowRssi24G = roaming.rssi_2g;
    }
    if(p_lowRssi5G != NULL)
    {
    	*p_lowRssi5G = roaming.rssi_5g;
    }
}

void get_macfilter_status(int* p_enable, int* p_policy, char* p_entryBuffer, int p_bufferSize)
{
    MacFilter macfilter;
    MACFILTER_T *macentry;
	int i = 0;
	char ver[33];
	apmib_get(MIB_RTL_LINK_ANDLINK_VER, (void *)ver);
	memset(&macfilter, 0, sizeof(macfilter));
	if (!strcasecmp(ver, "v2.10e"))
	{
        list_init(&macfilter.mac_list);
        rtl_link_getMacFilterSetting(&macfilter);
	}
	if(p_enable != NULL)
	{
		*p_enable = macfilter.enable;
	}
	if(p_policy != NULL)
	{
		*p_policy = macfilter.policy;
	}
	if(p_entryBuffer != NULL && p_bufferSize > 0)
	{
		char filter_item[64];
        if (macfilter.mac_list.len > 32)
        {
            macfilter.mac_list.len = 32;
        }
        for (i = 0; i < macfilter.mac_list.len; i++)
        {
            macentry = list_get_element(&macfilter.mac_list, i);
            snprintf(filter_item, sizeof(filter_item), "%02x:%02x:%02x:%02x:%02x:%02x/%s,",
                macentry->macAddr[0], macentry->macAddr[1], macentry->macAddr[2],
				macentry->macAddr[3], macentry->macAddr[4], macentry->macAddr[5],
				macentry->comment);
            strcat(p_entryBuffer, filter_item);
        }
	}
}

int get_radio_number()
{
	RfStatus rf;
    memset(&rf, 0, sizeof(rf));
    rtl_link_getRFInfo(&rf);
    return rf.num;
}

void get_radio_status(int index, char* p_radioName, int* p_radioEnable,
		char* p_radioPower, int* p_radioChannel)
{
	RfStatus rf;
    memset(&rf, 0, sizeof(rf));
    rtl_link_getRFInfo(&rf);

    if(index >= 0 && index < rf.num)
    {
    	if(p_radioName != NULL)
    	{
    		sprintf(p_radioName, "%s",
    				rf.rf[index].band == PHYBAND_2G ? "2.4G" : "5G");
    	}
    	if(p_radioEnable != NULL)
    	{
    		*p_radioEnable = rf.rf[index].enable;
    	}
    	if(p_radioPower != NULL)
    	{
    		sprintf(p_radioPower, "%d", rf.rf[index].power);
    	}
    	if(p_radioChannel != NULL)
    	{
    		*p_radioChannel = rf.rf[index].channel;
    	}
    }
}

#endif

