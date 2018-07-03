#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum class EventType
{
	E_EVENT_TYPE_UNKNOWN,
	E_EVENT_TYPE_ANDLINK_DEVICE,
	E_EVENT_TYPE_TERMINAL_INPUT
};

struct EventTypeDataObjectBase
{
	EventTypeDataObjectBase() = default;
	virtual ~EventTypeDataObjectBase(){ }
};

struct EventTypeTerminalDataObject: public EventTypeDataObjectBase
{
	EventTypeTerminalDataObject(std::string& p_rawData): m_rawData(p_rawData)
	{
		m_eventType = EventType::E_EVENT_TYPE_TERMINAL_INPUT;
	}
	~EventTypeTerminalDataObject()
	{

	}
	EventType m_eventType;
	std::string m_rawData;
};

struct EventTypeUDPClientDataObject: public EventTypeDataObjectBase
{
	EventTypeUDPClientDataObject(struct sockaddr_in p_cliAddr, int p_sockfd, std::string p_rawData)
		: m_clientAddr(p_cliAddr), m_serverSocketFd(p_sockfd), m_rawData(p_rawData)
	{
		m_cliSockLength = sizeof(struct sockaddr_in);
		m_eventType = EventType::E_EVENT_TYPE_ANDLINK_DEVICE;
	}
	~EventTypeUDPClientDataObject()
	{

	}
	struct sockaddr_in m_clientAddr;
	socklen_t m_cliSockLength;
	int m_serverSocketFd;
	std::string m_rawData;
	EventType m_eventType;
};

std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeTerminalDataObject> obj);
std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeUDPClientDataObject> obj);
