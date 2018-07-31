#include "UDPClient.hpp"
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>

UDPClient::UDPClient(std::string p_host, int p_port): m_host(p_host), m_port(p_port), m_sockfd(-1)
{

}

UDPClient::~UDPClient()
{

}

bool UDPClient::startUDPClient()
{
	shutDownUDPClient();

	int l_udpSocketfd = create_an_udp_socket_client(m_host, m_port);
	if(l_udpSocketfd < 0)
	{
		printf("create udp socket failed, %s\n", strerror(errno));
		return false;
	}
	m_sockfd = l_udpSocketfd;

	struct sockaddr_in l_localAddr;
	socklen_t len = sizeof(struct sockaddr_in);
	if(0 == getsockname(m_sockfd, (struct sockaddr*)(&l_localAddr), &len))
	{
		m_localPort = ntohs(l_localAddr.sin_port);
		m_localhost = inet_ntoa(l_localAddr.sin_addr);
	}
	printf("start UDP Client success, %s:%d\n", m_localhost.c_str(), m_localPort);
	return true;
}

void UDPClient::shutDownUDPClient()
{
	if(m_sockfd)
	{
		close(m_sockfd);
		m_sockfd = -1;
	}
}

int UDPClient::writeUDPString(std::string p_data)
{
	if(m_sockfd <= 0 || p_data.size() <= 0)
	{
		printf("write UDP string failed, m_sockfd=%d, data size=%d \n", m_sockfd, p_data.size());
		return 0;
	}
	struct sockaddr_in l_serverAddr;
	bzero(&l_serverAddr, sizeof(struct sockaddr_in));
	l_serverAddr.sin_family = AF_INET;
	l_serverAddr.sin_port = htons(m_port);
	l_serverAddr.sin_addr.s_addr = inet_addr(m_host.c_str());

	int l_writeSize = sendto(m_sockfd, p_data.c_str(), p_data.size(), 0,
			(struct sockaddr*)&(l_serverAddr), sizeof(struct sockaddr_in));
	if(l_writeSize > 0)
	{
		std::cout << "send msg size: " << l_writeSize << ", " << p_data << std::endl;
	}
	return l_writeSize;
}

std::string UDPClient::readUDPString(int p_timeout)
{
	if(m_sockfd <= 0)
	{
		return std::string();
	}
	fd_set l_sockfdSet;
	FD_ZERO(&l_sockfdSet);
	FD_SET(m_sockfd, &l_sockfdSet);
	struct timeval l_timeout  = {p_timeout, 0};
	struct timeval* l_timeoutPtr = NULL;
	if(p_timeout > 0)
	{
		l_timeoutPtr = &l_timeout;
	}
	switch(select(m_sockfd + 1, &l_sockfdSet, NULL, NULL, l_timeoutPtr))
	{
	case -1:
		break;
	case 0:
		break;
	default:
		char l_buffer[1024 * 8] = { 0 };
		memset(l_buffer, 0, sizeof(l_buffer));
		ssize_t l_nready = recv(m_sockfd, l_buffer, sizeof(l_buffer)-1, 0);
		if(l_nready > 0)
		{
			std::cout << "recv msg: " << l_buffer << std::endl;
			return std::string(l_buffer);
		}
		break;
	}
	return std::string();
}

std::string UDPClient::writeAndReadUDPString(std::string p_data)
{
	if(writeUDPString(p_data) > 0)
	{
		return readUDPString(2);
	}
	return std::string();
}

int UDPClient::create_an_udp_socket_client(std::string p_host, int p_port)
{
	int ret = 0;
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		printf("create_an_udp_socket_client - create udp socket failed\n");
		return -1;
	}

	struct sockaddr_in ser_addr;
	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(p_port);
	ser_addr.sin_addr.s_addr = inet_addr(p_host.c_str());

	ret = connect(sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in));
	if(ret < 0)
	{
		printf("create_an_udp_socket_client - udp socket connect failed\n");
		close(sockfd);
		return -1;
	}

	return sockfd;
}

std::string UDPClient::getLocalUDPHost()
{
	return m_localhost;
}

int UDPClient::getLocalUDPPort()
{
	return m_localPort;
}


