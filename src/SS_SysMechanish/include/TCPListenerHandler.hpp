/*
 * TCPListenerHandler.hpp
 *
 *  Created on: 2018Äê7ÔÂ3ÈÕ
 *      Author: Administrator
 */

#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "glog/logging.h"

struct ClientConnectInfo
{
	ClientConnectInfo(int p_sock, std::string p_ip, int p_port)
		: m_sockfd(p_sock), m_ip(p_ip), m_port(p_port)
	{

	}
	int m_sockfd;
	std::string m_ip;
	int m_port;
};

class AsynTCPListenerHandler
{
public:
	AsynTCPListenerHandler(std::string p_host, int p_port, std::function<bool(std::shared_ptr<ClientConnectInfo>)> p_callback);
	virtual ~AsynTCPListenerHandler();
	bool runTCPListener();
	void shutdownTCPListener();

private:
	void mainloop();
	int createTCPServerSocket(std::string p_host, int p_port, int p_blockNumber);
	int triggerMainloopAcceptNotBlock(std::string p_host, int p_port);

	const std::string m_host;
	const int m_port;
	std::thread m_thread;
	bool m_threadExitFlag;
	std::function<bool(std::shared_ptr<ClientConnectInfo>)> m_callback;
	int m_sockfd;
};


