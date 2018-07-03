/*
 * TerminalHandler.cpp
 *
 *  Created on: Jul 2, 2018
 *      Author: xiaofenh
 */

#include "TerminalHandler.hpp"
#include "glog/logging.h"
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int SyncTerminalHandler::writeTerminalString(std::string p_str)
{
	if(p_str.size() <= 0)
	{
		return 0;
	}
	return write(1, p_str.c_str(), p_str.size());
}

std::string SyncTerminalHandler::readTerminalString(int p_timeoutSec, int p_timeoutUsec)
{
	char l_buffer[1024 * 8] = { 0 };
	memset(l_buffer, 0, sizeof(l_buffer));

	if(std::fgets(l_buffer, sizeof(l_buffer), stdin))
	{
		return std::string(l_buffer);
	}
	return std::string();
}

AsynTerminalHandler::AsynTerminalHandler(std::function<bool(std::string)> p_callback)
	: m_threadExitFlag(false), m_callback(p_callback)
{
	LOG(INFO) << "construct AsynTerminalHandler";
}

AsynTerminalHandler::~AsynTerminalHandler()
{
	LOG(INFO) << "de-construct AsynTerminalHandler";
}

bool AsynTerminalHandler::runTerminal()
{
	std::function<void()> l_threadTask = std::bind(&AsynTerminalHandler::mainloop, this);
	m_thread = std::move(std::thread(l_threadTask));
	return true;
}

void AsynTerminalHandler::shutdownTerminal()
{
	std::fclose(stdin);
	m_threadExitFlag = true;
	m_thread.join();
}

int AsynTerminalHandler::writeTerminalString(std::string p_str)
{
	if(p_str.size() <= 0)
	{
		return 0;
	}
	return write(1, p_str.c_str(), p_str.size());
}

void AsynTerminalHandler::mainloop()
{
	LOG(INFO) << "AsynTerminalHandler::mainloop start";

	while(m_threadExitFlag == false)
	{
		char l_buffer[1024 * 8] = { 0 };
		memset(l_buffer, 0, sizeof(l_buffer));

		fd_set l_sockfd_set;
		FD_ZERO(&l_sockfd_set);
		FD_SET(0, &l_sockfd_set);

		struct timeval l_timeout  = {1, 0};

		switch(select(0 + 1, &l_sockfd_set, NULL, NULL, &l_timeout))
		{
		case -1:
			break;
		case 0:
			break;
		default:
			if(FD_ISSET(0, &l_sockfd_set))
			{
				char l_buffer[1024 * 8] = { 0 };
				memset(l_buffer, 0, sizeof(l_buffer));

				struct sockaddr_in cli_addr;
				socklen_t len = sizeof(struct sockaddr_in);
				ssize_t l_readNum = read(0, l_buffer, sizeof(l_buffer));
				if(l_readNum > 0)
				{
					m_callback(std::string(l_buffer));
					break;
				}
				else
				{
					break;
				}
			}
		}
	}
	LOG(INFO) << "AsynTerminalHandler::mainloop exit";
}
