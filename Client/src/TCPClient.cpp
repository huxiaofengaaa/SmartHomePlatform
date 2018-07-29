#include "TCPClient.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

TCPClient::TCPClient(std::string p_host, int p_port): m_host(p_host), m_port(p_port)
{

}

TCPClient::~TCPClient()
{

}

bool TCPClient::startTCPClient()
{
	int l_tcpSocketfd = create_an_tcp_socket_client(m_host, m_port);
	if(l_tcpSocketfd < 0)
	{
		return false;
	}
	m_sockfd = l_tcpSocketfd;

	struct sockaddr_in l_localAddr;
	socklen_t len = sizeof(struct sockaddr_in);
	if(0 == getsockname(m_sockfd, (struct sockaddr*)(&l_localAddr), &len))
	{
		m_localPort = ntohs(l_localAddr.sin_port);
		m_localhost = inet_ntoa(l_localAddr.sin_addr);
	}
	return true;
}

void TCPClient::shutDownTCPClient()
{
	if(m_sockfd)
	{
		close(m_sockfd);
		m_sockfd = -1;
	}
}

std::string TCPClient::getLocalTCPHost()
{
	return m_localhost;
}

int TCPClient::getLocalTCPPort()
{
	return m_localPort;
}

int TCPClient::create_an_tcp_socket_client(std::string p_host,int p_port)
{
	int ret = 0;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		printf("create_an_tcp_socket_client - socket SOCK_STREAM failed, %s:%d\n",
				p_host.c_str(), p_port);
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
		printf("create_an_tcp_socket_client - socket connect failed, %s:%d\n",
				p_host.c_str(), p_port);
		close(sockfd);
		return -1;
	}
	return sockfd;
}

int TCPClient::writeTCPString(std::string p_data)
{
	if(m_sockfd <= 0 || p_data.size() <= 0)
	{
		return 0;
	}

	int l_writeSize = send(m_sockfd, p_data.c_str(), p_data.size(), 0);
	if(l_writeSize > 0)
	{
		std::cout << "send msg: " << p_data << std::endl;
	}
	return l_writeSize;
}

std::string TCPClient::readTCPString(int p_timeout)
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

std::string TCPClient::writeAndReadTCPString(std::string p_data)
{
	if(writeTCPString(p_data) > 0)
	{
		return readTCPString(2);
	}
	return std::string();
}

