/*
 * UDPServerHandler.hpp
 *
 *  Created on: 2018Äê7ÔÂ3ÈÕ
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
#include "glog/logging.h"
#include "EventTypeStruct.hpp"

#pragma once

class AsynUDPServerHandler
{
public:
	AsynUDPServerHandler(std::string p_host, int p_port,
			std::function<bool(std::shared_ptr<EventTypeUDPClientDataObject>)> p_callback);
	virtual ~AsynUDPServerHandler();
	bool runUDPServer();
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
