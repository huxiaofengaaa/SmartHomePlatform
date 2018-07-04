/*
 * UDPServerHandler.hpp
 *
 *  Created on: 2018��7��3��
 *      Author: Administrator
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <thread>
#include <functional>
#include "glog/logging.h"

#pragma once

struct EventTypeUDPClientDataObject
{
	EventTypeUDPClientDataObject(std::string p_host, int p_port, int p_sockfd, std::string p_rawData)
		: m_host(p_host), m_port(p_port), m_serverSocketFd(p_sockfd), m_rawData(p_rawData)
	{

	}
	~EventTypeUDPClientDataObject()
	{

	}
	std::string m_host;
	int m_port;
	int m_serverSocketFd;
	std::string m_rawData;
};

std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeUDPClientDataObject> obj);

class AsynUDPServerHandler
{
public:
	AsynUDPServerHandler(std::string p_host, int p_port,
			std::function<bool(std::shared_ptr<EventTypeUDPClientDataObject>)> p_callback);
	virtual ~AsynUDPServerHandler();
	bool runUDPServer();
	ssize_t writeUDPServerString(std::shared_ptr<EventTypeUDPClientDataObject> resp);
	void shutdownUDPServer();
private:
	void mainloop();
	int createUDPServerSocket(std::string p_host, int p_port);

	int m_sockfd;
	const std::string m_host;
	const int m_port;

	std::thread m_thread;
	std::function<bool(std::shared_ptr<EventTypeUDPClientDataObject>)> m_callback;
	bool m_threadExitFlag;
};


