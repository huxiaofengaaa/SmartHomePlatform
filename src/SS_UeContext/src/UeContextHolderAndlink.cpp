#include "UeContextHolderAndlink.hpp"
#include "AndlinkDeviceEvent.hpp"
#include "RandomGenerator.hpp"
#include "CalendarClock.hpp"
#include "MD5.h"

UeContextHolderAndlink::UeContextHolderAndlink(
		std::string p_host, int p_port, std::shared_ptr<ConfigParser> p_config):
		m_host(p_host), m_port(p_port), m_config(p_config), DataBaseStorage()
{
	m_defaultEncryptType = m_config->getParamInteger("EncryptType", 0);

	std::string l_redisServerHost = m_config->getParamString("RedisServerHost", "127.0.0.1");
	int l_redisServerPort = m_config->getParamInteger("RedisServerPort", 6379);
	std::string l_redisServerPw = m_config->getParamString("RedisServerPasswd", "");

	LOG(INFO) << "Redis server is " << l_redisServerHost << ":" << l_redisServerPort
			  << ", password " << l_redisServerPw;

	if(true == DataBaseStorage::reStartup(l_redisServerHost, l_redisServerPort, l_redisServerPw))
	{
		if(true == recoveryAllDataFromRemoteServer())
		{
			LOG(INFO) << "recovery all data from remote server successfully";
		}
		else
		{
			LOG(INFO) << "recovery all data from remote server failed";
		}
	}
	else
	{
		LOG(INFO) << "data base storage re-startup failed";
	}
}

std::vector<std::string> UeContextHolderAndlink::getDeviceList()
{
	return getAllKey();
}

int UeContextHolderAndlink::onlineSize()
{
	int l_onlineNumber = 0;
	CalendarClock clock;
	long l_currentTimestamp = clock.getTimeStamp();
	for(auto l_key : getAllKey())
	{
		auto l_ueContext = getRef(l_key);
		auto l_isAuthCompleted = l_ueContext->isAuth;
		auto l_lastTimestamp = l_ueContext->lastHeartbeat;
		auto l_heartbeatInterval = l_ueContext->heartBeatTime;
		if(l_lastTimestamp > (l_currentTimestamp - l_heartbeatInterval * 2)
				&& l_isAuthCompleted == true)
		{
			l_onlineNumber++;
		}
	}
	return l_onlineNumber;
}

int UeContextHolderAndlink::pluginSize()
{
	int l_pluginNumber = 0;
	for(auto l_key : getAllKey())
	{
		auto l_ueContext = getRef(l_key);
		if(l_ueContext->isPlugIn == true)
		{
			l_pluginNumber++;
		}
	}
	return l_pluginNumber;
}

std::string UeContextHolderAndlink::getDeviceIDByMAC(std::string p_deviceMac)
{
	for(auto l_key : getAllKey())
	{
		auto l_keyValue = getRef(l_key);
		if(l_keyValue->deviceMac == p_deviceMac)
		{
			return l_keyValue->deviceId;
		}
	}
	return std::string();
}

std::string UeContextHolderAndlink::getDeviceIDByTCPAddress(std::string p_host, int p_port)
{
	for(auto l_key : getAllKey())
	{
		auto l_keyValue = getRef(l_key);
		if(l_keyValue->peerTCPHost == p_host
				&& l_keyValue->peerTCPPort == p_port)
		{
			return l_keyValue->deviceId;
		}
	}
	return std::string();
}

std::string UeContextHolderAndlink::getDeviceIDByUDPAddress(std::string p_host, int p_port)
{
	for(auto l_key : getAllKey())
	{
		auto l_keyValue = getRef(l_key);
		if(l_keyValue->peerUDPHost == p_host
				&& l_keyValue->peerUDPPort == p_port)
		{
			return l_keyValue->deviceId;
		}
	}
	return std::string();
}

bool UeContextHolderAndlink::shouldDeviceEncrypt(std::string p_deviceID)
{
	if(p_deviceID.empty() == false)
	{
		auto l_uecontext = getRef(p_deviceID);
		if(l_uecontext && l_uecontext->encrypt != 0)
		{
			return true;
		}
	}
	return false;
}

bool UeContextHolderAndlink::updateNetAddress(std::string p_deviceMAC,
		std::string p_host, int p_port, int p_sockfd, bool isTCP)
{
	std::string l_deviceID = getDeviceIDByMAC(p_deviceMAC);
	auto l_uecontext = getRef(l_deviceID);
	if(l_uecontext)
	{
		if(isTCP == true)
		{
			l_uecontext->peerTCPHost = p_host;
			l_uecontext->peerTCPPort = p_port;
			l_uecontext->TCPSocketfd = p_sockfd;
			l_uecontext->isPlugIn = true;
		}
		else
		{
			l_uecontext->peerUDPHost = p_host;
			l_uecontext->peerUDPPort = p_port;
			l_uecontext->UDPSocketfd = p_sockfd;
			l_uecontext->isPlugIn = false;
		}
		return true;
	}
	return false;
}

bool UeContextHolderAndlink::pluginDisconnect(int p_sockfd, std::string p_host, int p_port)
{
	for(auto l_key : getAllKey())
	{
		auto l_Context = getRef(l_key);
		if(l_Context->peerTCPHost == p_host
				&& l_Context->peerTCPPort == p_port
				&& l_Context->TCPSocketfd == p_sockfd)
		{
			l_Context->peerTCPHost = "";
			l_Context->peerTCPPort = -1;
			l_Context->TCPSocketfd = -1;
			l_Context->isPlugIn = false;
			return true;
		}
	}
	return false;
}

bool UeContextHolderAndlink::pluginResetStatus(std::string p_deviceID)
{
	auto l_Context = getRef(p_deviceID);
	if(!l_Context)
	{
		return false;
	}
	l_Context->isRegister = true;
	l_Context->isOnline = false;
	l_Context->isAuth = false;
	l_Context->isPlugIn = false;
	return true;
}

bool UeContextHolderAndlink::setRegisterResponse(std::string p_deviceID,
		struct Interface56_Register_Resp& resp)
{
	auto l_uecontext = getRef(p_deviceID);
	if(l_uecontext)
	{
		resp.gwToken = l_uecontext->gwToken;
		resp.deviceId = l_uecontext->deviceId;
		resp.deviceToken = l_uecontext->deviceToken;
		resp.andlinkToken = l_uecontext->andlinkToken;
		return true;
	}
	return false;
}

bool UeContextHolderAndlink::setOnlineResponse(std::string p_deviceID,
		struct Interface56_Online_Resp& resp, bool isSuccess)
{
	auto l_uecontext = getRef(p_deviceID);
	if(l_uecontext && true == isSuccess)
	{
		CalendarClock clock;
		resp.timestamp = clock.getTimeStamp();
		resp.respCode = 0;
		resp.encrypt = l_uecontext->encrypt;
		resp.ChallengeCode = l_uecontext->ChallengeCode;
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);
	}
	else
	{
		resp.respCode = -5;
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port);
	}
	return true;
}

bool UeContextHolderAndlink::setAuthResponse(bool p_result, struct Interface56_Auth_Resp& resp)
{
	if(p_result == true)
	{
		resp.respCode = 2;
		resp.heartBeatTime = 15;
		resp.MessageServer = m_host + std::string(":") + std::to_string(m_port);
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);
	}
	else
	{
		resp.respCode = -2;
		resp.MessageServer = m_host + std::string(":") + std::to_string(m_port);
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);
	}
	return true;
}

bool UeContextHolderAndlink::setHeartbeatResponse(bool p_result, struct Interface56_Heartbeat_Resp& resp)
{
	if(p_result == true)
	{
		resp.respCode = 0;
		resp.heartBeatTime = 15;
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);
	}
	else
	{
		resp.respCode = -5;
		resp.ServerIP = m_host + std::string(":") + std::to_string(m_port+1);
	}
	return false;
}

std::string UeContextHolderAndlink::DeviceRegister(std::string p_deviceMac,
		std::string p_deviceType, std::string p_productToken)
{
	auto l_currentDeviceID = getDeviceIDByMAC(p_deviceMac);
	if(l_currentDeviceID.empty() == true)
	{
		std::string deviceID = generatorDeviceID();
		auto l_obj = std::make_shared<UeContextAndlink>(deviceID);
		l_obj->deviceMac = p_deviceMac;
		l_obj->deviceType = p_deviceType;
		l_obj->productToken = p_productToken;
		l_obj->gwToken = generatorGwToken();
		l_obj->deviceToken = generatorDeviceToken();
		l_obj->andlinkToken = generatorAndlinkToken();

		l_obj->isRegister = true;
		l_obj->isOnline = false;
		l_obj->isAuth = false;
		l_obj->isPlugIn = false;
		add(deviceID, l_obj);
		LOG(INFO) << "create new UeContextAndlink " << deviceID;
		return deviceID;
	}
	else
	{
		auto l_uecontext = getRef(l_currentDeviceID);
		l_uecontext->deviceMac = p_deviceMac;
		l_uecontext->deviceType = p_deviceType;
		l_uecontext->productToken = p_productToken;
		l_uecontext->gwToken = generatorGwToken();
		l_uecontext->deviceToken = generatorDeviceToken();
		l_uecontext->andlinkToken = generatorAndlinkToken();

		l_uecontext->isRegister = true;
		return l_currentDeviceID;
	}
}

bool UeContextHolderAndlink::DeviceOnline(struct Interface56_Online_Req& p_onlinereq)
{
	RandomGenerator l_generator;
	auto l_deviceid = p_onlinereq.deviceId;
	auto l_uecontext = getRef(l_deviceid);
	if(!l_uecontext)
	{
		return false;
	}
	if(l_uecontext->deviceMac != p_onlinereq.deviceMac
			|| l_uecontext->deviceType != p_onlinereq.deviceType
			|| l_uecontext->isRegister == false)
	{
		return false;
	}

	l_uecontext->DevRND = p_onlinereq.DevRND;
	l_uecontext->firmwareVersion = p_onlinereq.firmwareVersion;
	l_uecontext->softwareVersion = p_onlinereq.softwareVersion;
	l_uecontext->ipAddress = p_onlinereq.ipAddress;
	l_uecontext->deviceVendor = p_onlinereq.deviceVendor;
	l_uecontext->deviceModel = p_onlinereq.deviceModel;
	l_uecontext->deviceSn = p_onlinereq.deviceSn;
	l_uecontext->apUplinkType = p_onlinereq.apUplinkType;
	l_uecontext->radio5 = p_onlinereq.radio5;
	l_uecontext->user_key = p_onlinereq.user_key;
	l_uecontext->SyncCode = p_onlinereq.SyncCode;

	l_uecontext->heartBeatTime = 15000;
	/*
	 * Set the default encryption type through the configuration file;
	 */
	l_uecontext->encrypt = m_defaultEncryptType;
	l_uecontext->ChallengeCode = l_generator.generatorRandomNumberString(16);

	l_uecontext->isOnline = true;
	return true;
}

bool UeContextHolderAndlink::DeviceAuth(std::string p_deviceMAC, std::string p_CheckSN)
{
	std::string l_deviceID = getDeviceIDByMAC(p_deviceMAC);
	if(l_deviceID.empty() == true)
	{
		LOG(INFO) << "deviceID is empty";
		return false;
	}
	auto l_uecontext = getRef(l_deviceID);

	std::string l_ChallengeCode = l_uecontext->ChallengeCode;
	std::string l_DeviceSn = l_uecontext->deviceSn;

	char l_md5Result[64] = { 0 };
	char l_srcString[256] = { 0 };
	snprintf(l_srcString, sizeof(l_srcString), "%s%s", l_ChallengeCode.c_str(), l_DeviceSn.c_str());
	Compute_string_md5(l_srcString, l_md5Result);

	if(p_CheckSN == l_md5Result)
	{
		l_uecontext->isAuth = true;
		return true;
	}
	else
	{
		LOG(INFO) << "device " << l_deviceID << " auth failed for error checkSN, "
				<< "ChallengeCode=" << l_ChallengeCode
				<< ",DeviceSn=" << l_DeviceSn
				<< ",CheckSN=" << p_CheckSN;
		l_uecontext->isAuth = false;
		return false;
	}
}

bool UeContextHolderAndlink::DeviceHeartbeat(std::string p_deviceId, std::string p_deviceMAC,
		std::string p_IPAddr)
{
	auto l_uecontext = getRef(p_deviceId);
	if(!l_uecontext || l_uecontext->deviceMac != p_deviceMAC
			|| l_uecontext->isDeviceOnline() == false)
	{
		return false;
	}
	CalendarClock clock;
	l_uecontext->lastHeartbeat = clock.getTimeStamp();
	l_uecontext->ipAddress = p_IPAddr;
	storeAndlinkDevice(p_deviceId);
	return true;
}

bool UeContextHolderAndlink::storeAndlinkDevice(std::string p_deviceId)
{
	auto l_uecontext = getRef(p_deviceId);
	if(!l_uecontext)
	{
		return false;
	}
	DataBaseStorage::hashWriteOneFieldBool(p_deviceId, "isRegister", l_uecontext->isRegister);
	DataBaseStorage::hashWriteOneFieldBool(p_deviceId, "isOnline", l_uecontext->isOnline);
	DataBaseStorage::hashWriteOneFieldBool(p_deviceId, "isAuth", l_uecontext->isAuth);
	DataBaseStorage::hashWriteOneFieldBool(p_deviceId, "isPlugIn", l_uecontext->isPlugIn);
	DataBaseStorage::hashWriteOneFieldNumber(p_deviceId, "lastHeartbeat", l_uecontext->lastHeartbeat);
	DataBaseStorage::hashWriteOneField(p_deviceId, "peerUDPHost", l_uecontext->peerUDPHost);
	DataBaseStorage::hashWriteOneFieldNumber(p_deviceId, "peerUDPPort", l_uecontext->peerUDPPort);
	DataBaseStorage::hashWriteOneFieldNumber(p_deviceId, "UDPSocketfd", l_uecontext->UDPSocketfd);
	DataBaseStorage::hashWriteOneField(p_deviceId, "peerTCPHost", l_uecontext->peerTCPHost);
	DataBaseStorage::hashWriteOneFieldNumber(p_deviceId, "peerTCPPort", l_uecontext->peerTCPPort);
	DataBaseStorage::hashWriteOneFieldNumber(p_deviceId, "TCPSocketfd", l_uecontext->TCPSocketfd);
	DataBaseStorage::hashWriteOneField(p_deviceId, "deviceId", l_uecontext->deviceId);
	DataBaseStorage::hashWriteOneField(p_deviceId, "deviceMac", l_uecontext->deviceMac);
	DataBaseStorage::hashWriteOneField(p_deviceId, "deviceType", l_uecontext->deviceType);
	DataBaseStorage::hashWriteOneField(p_deviceId, "productToken", l_uecontext->productToken);
	DataBaseStorage::hashWriteOneField(p_deviceId, "gwToken", l_uecontext->gwToken);
	DataBaseStorage::hashWriteOneField(p_deviceId, "deviceToken", l_uecontext->deviceToken);
	DataBaseStorage::hashWriteOneField(p_deviceId, "andlinkToken", l_uecontext->andlinkToken);
	DataBaseStorage::hashWriteOneField(p_deviceId, "DevRND", l_uecontext->DevRND);
	DataBaseStorage::hashWriteOneField(p_deviceId, "firmwareVersion", l_uecontext->firmwareVersion);
	DataBaseStorage::hashWriteOneField(p_deviceId, "softwareVersion", l_uecontext->softwareVersion);
	DataBaseStorage::hashWriteOneField(p_deviceId, "ipAddress", l_uecontext->ipAddress);
	DataBaseStorage::hashWriteOneField(p_deviceId, "deviceVendor", l_uecontext->deviceVendor);
	DataBaseStorage::hashWriteOneField(p_deviceId, "deviceModel", l_uecontext->deviceModel);
	DataBaseStorage::hashWriteOneField(p_deviceId, "deviceSn", l_uecontext->deviceSn);
	DataBaseStorage::hashWriteOneField(p_deviceId, "apUplinkType", l_uecontext->apUplinkType);
	DataBaseStorage::hashWriteOneFieldNumber(p_deviceId, "radio5", l_uecontext->radio5);
	DataBaseStorage::hashWriteOneField(p_deviceId, "user_key", l_uecontext->user_key);
	DataBaseStorage::hashWriteOneField(p_deviceId, "SyncCode", l_uecontext->SyncCode);
	DataBaseStorage::hashWriteOneFieldNumber(p_deviceId, "heartBeatTime", l_uecontext->heartBeatTime);
	DataBaseStorage::hashWriteOneFieldNumber(p_deviceId, "encrypt", l_uecontext->encrypt);
	DataBaseStorage::hashWriteOneField(p_deviceId, "ChallengeCode", l_uecontext->ChallengeCode);

	DataBaseStorage::setAdd(c_deviceListKey, p_deviceId);
	return true;
}

bool UeContextHolderAndlink::recoveryAllDataFromRemoteServer()
{
	auto l_deviceList = DataBaseStorage::setGetAll(c_deviceListKey);
	for(auto iter = l_deviceList.begin() ; iter != l_deviceList.end(); iter++)
	{
		std::string l_deviceID = *iter;
		auto l_uecontext = getRef(l_deviceID);
		if(!l_uecontext)
		{
			l_uecontext = std::make_shared<UeContextAndlink>(l_deviceID);
			add(l_deviceID, l_uecontext);
		}

		l_uecontext->isRegister = DataBaseStorage::hashReadOneFieldBool(l_deviceID, "isRegister");
		l_uecontext->isOnline = DataBaseStorage::hashReadOneFieldBool(l_deviceID, "isOnline");
		l_uecontext->isAuth = DataBaseStorage::hashReadOneFieldBool(l_deviceID, "isAuth");
		l_uecontext->isPlugIn = DataBaseStorage::hashReadOneFieldBool(l_deviceID, "isPlugIn");
		l_uecontext->lastHeartbeat = DataBaseStorage::hashReadOneFieldNumber(l_deviceID, "lastHeartbeat");
		l_uecontext->peerUDPHost = DataBaseStorage::hashReadOneField(l_deviceID, "peerUDPHost");
		l_uecontext->peerUDPPort = DataBaseStorage::hashReadOneFieldNumber(l_deviceID, "peerUDPPort");
		l_uecontext->UDPSocketfd = DataBaseStorage::hashReadOneFieldNumber(l_deviceID, "UDPSocketfd");
		l_uecontext->peerTCPHost = DataBaseStorage::hashReadOneField(l_deviceID, "peerTCPHost");
		l_uecontext->peerTCPPort = DataBaseStorage::hashReadOneFieldNumber(l_deviceID, "peerTCPPort");
		l_uecontext->TCPSocketfd = DataBaseStorage::hashReadOneFieldNumber(l_deviceID, "TCPSocketfd");
		//l_uecontext->deviceId = DataBaseStorage::hashReadOneField(l_deviceID, "deviceId");
		l_uecontext->deviceMac = DataBaseStorage::hashReadOneField(l_deviceID, "deviceMac");
		l_uecontext->deviceType = DataBaseStorage::hashReadOneField(l_deviceID, "deviceType");
		l_uecontext->productToken = DataBaseStorage::hashReadOneField(l_deviceID, "productToken");
		l_uecontext->gwToken = DataBaseStorage::hashReadOneField(l_deviceID, "gwToken");
		l_uecontext->deviceToken = DataBaseStorage::hashReadOneField(l_deviceID, "deviceToken");
		l_uecontext->andlinkToken = DataBaseStorage::hashReadOneField(l_deviceID, "andlinkToken");
		l_uecontext->DevRND = DataBaseStorage::hashReadOneField(l_deviceID, "DevRND");
		l_uecontext->firmwareVersion = DataBaseStorage::hashReadOneField(l_deviceID, "firmwareVersion");
		l_uecontext->softwareVersion = DataBaseStorage::hashReadOneField(l_deviceID, "softwareVersion");
		l_uecontext->ipAddress = DataBaseStorage::hashReadOneField(l_deviceID, "ipAddress");
		l_uecontext->deviceVendor = DataBaseStorage::hashReadOneField(l_deviceID, "deviceVendor");
		l_uecontext->deviceModel = DataBaseStorage::hashReadOneField(l_deviceID, "deviceModel");
		l_uecontext->deviceSn = DataBaseStorage::hashReadOneField(l_deviceID, "deviceSn");
		l_uecontext->apUplinkType = DataBaseStorage::hashReadOneField(l_deviceID, "apUplinkType");
		l_uecontext->radio5 = DataBaseStorage::hashReadOneFieldNumber(l_deviceID, "radio5");
		l_uecontext->user_key = DataBaseStorage::hashReadOneField(l_deviceID, "user_key");
		l_uecontext->SyncCode = DataBaseStorage::hashReadOneField(l_deviceID, "SyncCode");
		l_uecontext->heartBeatTime = DataBaseStorage::hashReadOneFieldNumber(l_deviceID, "heartBeatTime");
		l_uecontext->encrypt = DataBaseStorage::hashReadOneFieldNumber(l_deviceID, "encrypt");
		l_uecontext->ChallengeCode = DataBaseStorage::hashReadOneField(l_deviceID, "ChallengeCode");

		LOG(INFO) << "recovery " << *iter;
	}
	return true;
}

std::string UeContextHolderAndlink::generatorDeviceID()
{
	RandomGenerator l_generator;
	std::string deviceID;
	do
	{
		deviceID = "CMCC-" + l_generator.generatorRandomNumberString(5)
				+ "-" + l_generator.generatorRandomCharString(12);
	}
	while(true == isExist(deviceID));
	return deviceID;
}

std::string UeContextHolderAndlink::generatorDeviceToken()
{
	RandomGenerator l_generator;
	return l_generator.generatorRandomCharString(32);
}

std::string UeContextHolderAndlink::generatorAndlinkToken()
{
	RandomGenerator l_generator;
	return l_generator.generatorRandomCharString(32);
}

std::string UeContextHolderAndlink::generatorGwToken()
{
	RandomGenerator l_generator;
	return l_generator.generatorRandomCharString(32);
}
