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

int SyncTerminalHandler::writeString(std::string p_str)
{
	if(p_str.size() <= 0)
	{
		return 0;
	}
	return write(1, p_str.c_str(), p_str.size());
}

std::string SyncTerminalHandler::readString(int p_timeoutSec, int p_timeoutUsec)
{
	int l_readfd = 0;
	fd_set l_sockfd_set;
	FD_ZERO(&l_sockfd_set);
	FD_SET(l_readfd, &l_sockfd_set);
	struct timeval l_timeout  =
	{
			p_timeoutSec,
			p_timeoutUsec
	};
	switch(select(l_readfd + 1, &l_sockfd_set, NULL, NULL, &l_timeout))
	{
	case -1:
		return std::string("selectError");
	case 0:
		return std::string("timeout");
	default:
		if(FD_ISSET(l_readfd, &l_sockfd_set))
		{
			char l_buffer[1024 * 8] = { 0 };
			memset(l_buffer, 0, sizeof(l_buffer));
			ssize_t l_readNum = read(l_readfd, l_buffer, sizeof(l_buffer));
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

AsynTerminalHandler::AsynTerminalHandler(std::function<bool(std::string)> p_callback)
	: m_threadExitFlag(false), m_callback(p_callback)
{
	m_threadExitFlag = true;
	m_thread.join();
	LOG(INFO) << "construct AsynTerminalHandler";
}

AsynTerminalHandler::~AsynTerminalHandler()
{
	LOG(INFO) << "de-construct AsynTerminalHandler";
}

bool AsynTerminalHandler::run()
{
	std::function<void()> l_threadTask = std::bind(&AsynTerminalHandler::mainloop, this);
	m_thread = std::thread(l_threadTask);
	return true;
}

int AsynTerminalHandler::writeString(std::string p_str)
{
	if(p_str.size() <= 0)
	{
		return 0;
	}
	return write(1, p_str.c_str(), p_str.size());
}

void AsynTerminalHandler::mainloop()
{
	int l_readfd = 0;
	fd_set l_sockfd_set;
	FD_ZERO(&l_sockfd_set);
	FD_SET(l_readfd, &l_sockfd_set);
	struct timeval l_timeout  = {1, 0};

	LOG(INFO) << "AsynTerminalHandler::mainloop start";

	while(m_threadExitFlag == false)
	{
		switch(select(l_readfd + 1, &l_sockfd_set, NULL, NULL, &l_timeout))
		{
		case -1:
			m_callback(std::string("selectError"));
			break;
		case 0:
			break;
		default:
			if(FD_ISSET(l_readfd, &l_sockfd_set))
			{
				char l_buffer[1024 * 8] = { 0 };
				memset(l_buffer, 0, sizeof(l_buffer));
				ssize_t l_readNum = read(l_readfd, l_buffer, sizeof(l_buffer));
				if(l_readNum > 0)
				{
					m_callback(std::string(l_buffer));
				}
				else
				{
					m_callback(std::string("readError"));
				}
			}
		}

	}
	LOG(INFO) << "AsynTerminalHandler::mainloop exit";
}
