/*
 * UDPClientHandler.cpp
 *
 *  Created on: 2018Äê7ÔÂ17ÈÕ
 *      Author: Administrator
 */
#include "UDPClientHandler.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

UDPClientHandler::UDPClientHandler(std::string p_host, int p_port, std::function<bool(std::string)> p_callback)
	: m_host(p_host), m_port(p_port), m_callback(p_callback)
{

}

UDPClientHandler::~UDPClientHandler()
{

}

ssize_t UDPClientHandler::writeUDPServerString(std::string p_data)
{
	struct sockaddr_in cli_addr;
	bzero(&cli_addr, sizeof(struct sockaddr_in));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(m_port);
	cli_addr.sin_addr.s_addr = inet_addr(m_host.c_str());

	return sendto(m_sockfd, p_data.c_str(), p_data.size(), 0,
			(struct sockaddr*)&(cli_addr), sizeof(struct sockaddr_in));
}

void UDPClientHandler::setNeedResponseFlag()
{
	m_responseMutex.lock();
	m_needResponse = true;
	m_response = std::string();
	m_responseMutex.unlock();
}

void UDPClientHandler::reSetNeedResponseFlag()
{
	m_responseMutex.lock();
	m_needResponse = false;
	m_responseMutex.unlock();
}

std::string UDPClientHandler::writeUDPServerAndGetResponse(std::string p_data)
{
	setNeedResponseFlag();

	if(writeUDPServerString(p_data) < 0)
	{
		return std::string();
	}

	struct timespec abs_timeout;
	bzero(&abs_timeout, 0);
	clock_gettime(CLOCK_REALTIME, &abs_timeout);
	abs_timeout.tv_sec += 2;
	long l_seeting_time = time(NULL);
	int ret = sem_timedwait(&m_getResponse_sem, &abs_timeout);
	if(ret != 0)
	{
		reSetNeedResponseFlag();
		return std::string();
	}
	reSetNeedResponseFlag();
	return m_response;
}

bool UDPClientHandler::runUDPClient()
{
	sem_init(&m_getResponse_sem, 0, 0);
	m_sockfd = createUDPClientSocket(m_host, m_port);
	if(m_sockfd < 0)
	{
		return false;
	}
	std::function<void()> l_threadTask = std::bind(&UDPClientHandler::mainloop, this);
	m_thread = std::move(std::thread(l_threadTask));
	return true;
}

void UDPClientHandler::shutDownUDPClient()
{
	m_threadExitFlag = true;
	m_thread.join();
	if(m_sockfd > 0)
	{
		close(m_sockfd);
		m_sockfd = -1;
	}
	sem_destroy(&m_getResponse_sem);
}

int UDPClientHandler::createUDPClientSocket(std::string p_host, int p_port)
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		return -1;
	}
	struct sockaddr_in ser_addr;
	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(p_port);
	ser_addr.sin_addr.s_addr = inet_addr(p_host.c_str());
	if(0 != connect(sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in)))
	{
		close(sockfd);
		return -1;
	}
	return sockfd;
}

bool UDPClientHandler::readDataFromServer(int p_sockfd)
{
	char readbuffer[1024 * 8] = { 0 };
	memset(readbuffer, 0, sizeof(readbuffer));
	ssize_t l_nready = recv(p_sockfd, readbuffer, sizeof(readbuffer)-1, 0);
	if(l_nready <= 0)
	{
		return false;
	}
	else
	{
		struct sockaddr_in l_cliaddr;
		socklen_t l_len = sizeof(struct sockaddr_in);
		std::string l_peerHost;
		int l_peerPort = -1;
		if(0 == getpeername(p_sockfd, (struct sockaddr*)&l_cliaddr, &l_len))
		{
			l_peerHost = inet_ntoa(l_cliaddr.sin_addr);
			l_peerPort = ntohs(l_cliaddr.sin_port);
		}
		if(m_needResponse)
		{
			m_response = std::string(readbuffer);
			sem_post(&m_getResponse_sem);
		}

		m_callback(std::string(readbuffer));
		return true;
	}
}

void UDPClientHandler::mainloop()
{
	while(m_threadExitFlag == false)
	{
		fd_set l_sockfd_set;
		FD_ZERO(&l_sockfd_set);
		FD_SET(m_sockfd, &l_sockfd_set);
		struct timeval l_timeout  = {1, 0};
		switch(select(m_sockfd + 1, &l_sockfd_set, NULL, NULL, &l_timeout))
		{
		case -1:
			break;
		case 0:
			break;
		default:
			if(FD_ISSET(m_sockfd, &l_sockfd_set))
			{
				readDataFromServer(m_sockfd);
			}
			break;
		}
	}
}
