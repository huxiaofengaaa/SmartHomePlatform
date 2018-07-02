#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum class ANDLINKDeviceDataType
{
	E_ANDLINK_DEVICE_REGISTER_REQ,
	E_ANDLINK_DEVICE_REGISTER_RESP,
	E_ANDLINK_DEVICE_ONLINE_REQ,
	E_ANDLINK_DEVICE_ONLINE_RESP,
	E_ANDLINK_DEVICE_AUTH_REQ,
	E_ANDLINK_DEVICE_AUTH_RESP,
	E_ANDLINK_DEVICE_HEARTBEAT_REQ,
	E_ANDLINK_DEVICE_HEARTBEAT_RESP,
	E_ANDLINK_DEVICE_REQUESTPLUGIN_REQ,
	E_ANDLINK_DEVICE_REQUESTPLUGIN_RESP
};

enum class EventType
{
	E_EVENT_TYPE_UNKNOWN,
	E_EVENT_TYPE_ANDLINK_DEVICE,
	E_EVENT_TYPE_TERMINAL_INPUT
};

struct EventTypeDataObject
{
	EventTypeDataObject(EventType p_type, const char* p_rawdata, int p_size);
	~EventTypeDataObject();

	EventType m_eventType;
	char* m_rawData;
};

struct EventTypeNetworkDataObject
{
	EventTypeNetworkDataObject(struct sockaddr_in p_cliAddr, int p_sockfd, std::string p_rawData)
		: m_clientAddr(p_cliAddr), m_cliSockLength(sizeof(struct sockaddr_in)), m_serverSocketFd(p_sockfd),
		  m_rawData(p_rawData), m_eventType(EventType::E_EVENT_TYPE_ANDLINK_DEVICE)
	{

	}
	~EventTypeNetworkDataObject()
	{

	}
	struct sockaddr_in m_clientAddr;
	socklen_t m_cliSockLength;
	int m_serverSocketFd;
	std::string m_rawData;
	EventType m_eventType;
};

std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeDataObject> obj);
std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeNetworkDataObject> obj);
