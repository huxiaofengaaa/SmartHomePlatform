/*
 * UDPServerHandler.cpp
 *
 *  Created on: 2018Äê7ÔÂ3ÈÕ
 *      Author: Administrator
 */

#include "UDPServerHandler.hpp"

AsynUDPServerHandler::AsynUDPServerHandler(std::string p_host, int p_port,
		std::function<bool(std::shared_ptr<EventTypeUDPClientDataObject>)> p_callback)
	: m_sockfd(-1), m_host(p_host), m_port(p_port), m_callback(p_callback), m_threadExitFlag(false)
  {
	LOG(INFO) << "construct AsynUDPServerHandler";
  }
AsynUDPServerHandler::~AsynUDPServerHandler()
{
	LOG(INFO) << "de-construct AsynUDPServerHandler";
}
bool AsynUDPServerHandler::runUDPServer()
{
	m_sockfd = createUDPServerSocket(m_host, m_port);
	if(m_sockfd < 0)
	{
		return false;
	}
	std::function<void()> l_threadTask = std::bind(&AsynUDPServerHandler::mainloop, this);
	m_thread = std::move(std::thread(l_threadTask));
	return true;
}
void AsynUDPServerHandler::shutdownUDPServer()
{
	m_threadExitFlag = true;
	m_thread.join();
	if(m_sockfd > 0)
	{
		close(m_sockfd);
		m_sockfd = -1;
	}
}

void AsynUDPServerHandler::mainloop()
{
	LOG(INFO) << "AsynUDPServerHandler::mainloop start";

	while(m_threadExitFlag == false)
	{
		char l_buffer[1024 * 8] = { 0 };
		memset(l_buffer, 0, sizeof(l_buffer));

		std::shared_ptr<EventTypeUDPClientDataObject> l_data;

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
				char l_buffer[1024 * 8] = { 0 };
				memset(l_buffer, 0, sizeof(l_buffer));

				struct sockaddr_in cli_addr;
				socklen_t len = sizeof(struct sockaddr_in);
				ssize_t l_readNum = recvfrom(m_sockfd, l_buffer, sizeof(l_buffer), 0, (struct sockaddr *)&(cli_addr), &(len));
				if(l_readNum > 0)
				{
					l_data = std::make_shared<EventTypeUDPClientDataObject>(
							inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), m_sockfd, std::string(l_buffer));
					break;
				}
				else
				{
					break;
				}
			}
		}
		if(l_data.get())
		{
			m_callback(l_data);
		}
	}
	LOG(INFO) << "AsynUDPServerHandler::mainloop exit";
}

ssize_t AsynUDPServerHandler::writeUDPServerString(std::shared_ptr<EventTypeUDPClientDataObject> resp)
{
	struct sockaddr_in cli_addr;
	bzero(&cli_addr, sizeof(struct sockaddr_in));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(resp->m_port);
	cli_addr.sin_addr.s_addr = inet_addr(resp->m_host.c_str());

	return sendto(m_sockfd, resp->m_rawData.c_str(), resp->m_rawData.size(), 0,
			(struct sockaddr*)&(cli_addr), sizeof(struct sockaddr_in));
}

int AsynUDPServerHandler::createUDPServerSocket(std::string p_host, int p_port)
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
