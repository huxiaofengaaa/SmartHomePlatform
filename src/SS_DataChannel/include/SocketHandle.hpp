#pragma once

#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <memory>

struct NetworkData
{
	struct sockaddr_in m_clientAddr;
	socklen_t m_sockLength;
	int m_fd;
	std::string m_rawData;
};

int createUDPServerSocket(std::string p_host, int p_port);
int createUDPClientSocket(std::string p_host, int p_port);
int createTCPServerSocket(std::string p_host, int p_port);
int createTCPClientSocket(std::string p_host, int p_port);

std::string readDataFromFdWithTimeout(int p_fd, int p_timeoutSec, int p_timeoutUsec);
std::shared_ptr<NetworkData> readDataFromSocketFdWithTimeout(int p_fd, int p_timeoutSec, int p_timeoutUsec);
ssize_t writeDataToFd(int p_fd, std::string p_data);
ssize_t writeDataToSocketFd(int p_fd, struct sockaddr_in p_addr, std::string p_data);
