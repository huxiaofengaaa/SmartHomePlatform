/*
 * TCPListenerHandler.cpp
 *
 *  Created on: 2018��7��3��
 *      Author: Administrator
 */

#include "TCPListenerHandler.hpp"

AsynTCPListenerHandler::AsynTCPListenerHandler(
		std::string p_host,
		int p_port,
		std::function<bool(std::shared_ptr<ClientConnectInfo>)> p_callback,
		int p_blockListNumber)
	: m_host(p_host),
	  m_port(p_port),
	  m_callback(p_callback),
	  m_blockListNumber(p_blockListNumber),
	  m_sockfd(-1),
	  m_threadExitFlag(false)
{
	LOG(INFO) << "construct AsynTCPListenerHandler";
}

AsynTCPListenerHandler::~AsynTCPListenerHandler()
{
	LOG(INFO) << "de-construct AsynTCPListenerHandler";
}

bool AsynTCPListenerHandler::runTCPListener()
{
	m_sockfd = createTCPServerSocket(m_host, m_port, m_blockListNumber);
	if(m_sockfd < 0)
	{
		return false;
	}
	std::function<void()> l_threadTask = std::bind(&AsynTCPListenerHandler::mainloop, this);
	m_thread = std::move(std::thread(l_threadTask));
	return true;
}

void AsynTCPListenerHandler::shutdownTCPListener()
{
	try
	{
		m_threadExitFlag = true;
		int l_tmpSocket = triggerMainloopAcceptNotBlock(m_host, m_port);
		if(l_tmpSocket >= 0)
		{
			m_thread.join();
			close(l_tmpSocket);
		}
		else
		{
			LOG(INFO) << "AsynTCPListenerHandler::shutdownTCPListener triggerMainloopAcceptNotBlock failed";
		}
		if(m_sockfd > 0)
		{
			close(m_sockfd);
			m_sockfd = -1;
		}
	}
	catch(std::exception& e)
	{
		LOG(INFO) << "AsynUDPServerHandler::shutdownUDPServer catch exception, " << e.what();
	}
	LOG(INFO) << "AsynTCPListenerHandler shutdown";
}

void AsynTCPListenerHandler::mainloop()
{
	LOG(INFO) << "AsynTCPListenerHandler main loop start";
	while(m_threadExitFlag == false)
	{
		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(struct sockaddr_in);

		int l_clisockfd = accept(m_sockfd, (struct sockaddr*)&cli_addr, &len);
		if(l_clisockfd >= 0 && m_threadExitFlag == false)
		{
			int l_port = ntohs(cli_addr.sin_port);
			std::string l_ip = inet_ntoa(cli_addr.sin_addr);
			m_callback(std::make_shared<ClientConnectInfo>(l_clisockfd, l_ip, l_port));
		}
	}
	LOG(INFO) << "AsynTCPListenerHandler main loop exit";
}

int AsynTCPListenerHandler::createTCPServerSocket(std::string p_host, int p_port, int p_blockNumber)
{
	int l_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(l_socketfd < 0)
	{
		return -1;
	}

	struct sockaddr_in ser_addr;
	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(p_port);
	ser_addr.sin_addr.s_addr = inet_addr(p_host.c_str());
	if(0 != bind(l_socketfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in)))
	{
		close(l_socketfd);
		return -1;
	}

	if(0 != listen(l_socketfd, p_blockNumber))
	{
		close(l_socketfd);
		return -1;
	}
	return l_socketfd;
}

int AsynTCPListenerHandler::triggerMainloopAcceptNotBlock(std::string p_host, int p_port)
{
	int l_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(l_socketfd < 0)
	{
		return -1;
	}
	struct sockaddr_in ser_addr;
	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(p_port);
	ser_addr.sin_addr.s_addr = inet_addr(p_host.c_str());

	if(0 != connect(l_socketfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in)))
	{
		close(l_socketfd);
		return -1;
	}
	return l_socketfd;
}

std::ostream& operator<<(std::ostream& os, std::shared_ptr<ClientConnectInfo> p_connectionInfo)
{
	os << "client connect infomation, IP:" << p_connectionInfo->m_ip
			<< " Port:" << p_connectionInfo->m_port
			<< " Sockfd:" << p_connectionInfo->m_sockfd;
	return os;
}
