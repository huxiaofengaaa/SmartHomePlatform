/*
 * TCPServerHandler.hpp
 *
 *  Created on: Jul 4, 2018
 *      Author: xiaofenh
 */

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <map>
#include <mutex>
#include "glog/logging.h"
#include "TCPListenerHandler.hpp"
#include <poll.h>

struct EventTypeTCPClientDataObject
{
	EventTypeTCPClientDataObject(int p_fd, std::string p_host, int p_port, std::string p_str)
		: m_socketfd(p_fd), m_host(p_host), m_port(p_port), m_rawData(p_str)
	{

	}
	int m_socketfd;
	std::string m_host;
	int m_port;
	std::string m_rawData;
};

std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeTCPClientDataObject> p_connectionInfo);

class AsynTCPServerHandler
{
public:
	AsynTCPServerHandler(std::function<bool(std::shared_ptr<EventTypeTCPClientDataObject>)> p_callback,
			std::function<bool(std::shared_ptr<ClientConnectInfo>)> p_closecallback);
	~AsynTCPServerHandler();
	bool runTCPServer();
	ssize_t writeTCPServerString(std::shared_ptr<EventTypeTCPClientDataObject> p_msg);
	void shutdownTCPServer();
	bool addClient(std::shared_ptr<ClientConnectInfo> p_cli);
	bool delClient(int p_fd);
private:
	void mainloop();

	std::thread m_thread;
	std::function<bool(std::shared_ptr<EventTypeTCPClientDataObject>)> m_datacallback;
	std::function<bool(std::shared_ptr<ClientConnectInfo>)> m_closecallback;
	bool m_threadExitFlag;

	std::vector<std::shared_ptr<ClientConnectInfo>> m_clientReadyToAdd;
	std::vector<int> m_clientReadyToDel;
	std::map<int, std::shared_ptr<ClientConnectInfo>> m_clientInfoMap;
	std::mutex m_clientInfoMutex;
};
