#include "SocketHandle.hpp"
#include <sys/select.h>
#include <string.h>

int createUDPServerSocket(std::string p_host, int p_port)
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("create socket failed\n");
		return -1;
	}

	struct sockaddr_in ser_addr;
	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(p_port);
	ser_addr.sin_addr.s_addr = inet_addr(p_host.c_str());
	if(0 != bind(sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in)))
	{
		close(sockfd);
		return -1;
	}

	return sockfd;
}

int createUDPClientSocket(std::string p_host, int p_port)
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("create socket failed\n");
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

int createTCPServerSocket(std::string p_host, int p_port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("create socket failed\n");
		return -1;
	}

	struct sockaddr_in ser_addr;
	bzero(&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(p_port);
	ser_addr.sin_addr.s_addr = inet_addr(p_host.c_str());
	if(0 != bind(sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in)))
	{
		close(sockfd);
		return -1;
	}

	return sockfd;
}

int createTCPClientSocket(std::string p_host, int p_port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("create socket failed\n");
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

std::shared_ptr<NetworkData> readDataFromSocketFdWithTimeout(int p_fd, int p_timeoutSec, int p_timeoutUsec)
{
	std::shared_ptr<NetworkData> l_data = std::make_shared<NetworkData>();
	if(p_fd < 0)
	{
		return l_data;
	}
	fd_set l_sockfd_set;
	FD_ZERO(&l_sockfd_set);
	FD_SET(p_fd, &l_sockfd_set);
	struct timeval l_timeout  = {p_timeoutSec, p_timeoutUsec};
	switch(select(p_fd + 1, &l_sockfd_set, NULL, NULL, &l_timeout))
	{
	case -1:
		l_data->m_rawData = std::string("selectError");
		break;
	case 0:
		l_data->m_rawData = std::string("timeout");
		break;
	default:
		if(FD_ISSET(p_fd, &l_sockfd_set))
		{
			char l_buffer[1024 * 8] = { 0 };
			memset(l_buffer, 0, sizeof(l_buffer));

			memset(&(l_data->m_clientAddr), 0, sizeof(struct sockaddr_in));
			l_data->m_sockLength = sizeof(struct sockaddr_in);

			ssize_t l_readNum = recvfrom(p_fd, l_buffer, sizeof(l_buffer), 0,
					(struct sockaddr *)&(l_data->m_clientAddr), &(l_data->m_sockLength));
			if(l_readNum > 0)
			{
				l_data->m_fd = p_fd;
				l_data->m_rawData = std::string(l_buffer);
				break;
			}
			else
			{
				l_data->m_rawData = std::string("readError");
				break;
			}
		}
	}

	return l_data;
}

std::string readDataFromFdWithTimeout(int p_fd, int p_timeoutSec, int p_timeoutUsec)
{
	if(p_fd < 0)
	{
		return std::string();
	}
	fd_set l_sockfd_set;
	FD_ZERO(&l_sockfd_set);
	FD_SET(p_fd, &l_sockfd_set);
	struct timeval l_timeout  = {p_timeoutSec, p_timeoutUsec};
	switch(select(p_fd + 1, &l_sockfd_set, NULL, NULL, &l_timeout))
	{
	case -1:
		return std::string("selectError");
	case 0:
		return std::string("timeout");
	default:
		if(FD_ISSET(p_fd, &l_sockfd_set))
		{
			char l_buffer[1024 * 8] = { 0 };
			memset(l_buffer, 0, sizeof(l_buffer));
			ssize_t l_readNum = read(p_fd, l_buffer, sizeof(l_buffer));
			if(l_readNum > 0)
			{
				return std::string(l_buffer);
			}
			else
			{
				return std::string("readError");
			}
		}
	}

	return std::string("CodeError");
}

ssize_t writeDataToSocketFd(int p_fd, struct sockaddr_in p_addr, std::string p_data)
{
	return sendto(p_fd, p_data.c_str(), p_data.size(), 0, (struct sockaddr*)&p_addr, sizeof(struct sockaddr_in));
}

ssize_t writeDataToFd(int p_fd, std::string p_data)
{
	return write(p_fd, p_data.c_str(), p_data.size());
}
