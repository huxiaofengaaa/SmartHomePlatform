/*
 * TCPServerHandler.cpp
 *
 *  Created on: Jul 4, 2018
 *      Author: xiaofenh
 */
#include "TCPServerHandler.hpp"

AsynTCPServerHandler::AsynTCPServerHandler(
		std::function<bool(std::shared_ptr<EventTypeTCPClientDataObject>)> p_datacallback,
		std::function<bool(std::shared_ptr<ClientConnectInfo>)> p_closecallback)
	: m_datacallback(p_datacallback), m_closecallback(p_closecallback), m_threadExitFlag(false)
{
	LOG(INFO) << "construct AsynTCPServerHandler";
}

AsynTCPServerHandler::~AsynTCPServerHandler()
{
	LOG(INFO) << "de-construct AsynTCPServerHandler";
}

bool AsynTCPServerHandler::runTCPServer()
{
	std::function<void()> l_threadTask = std::bind(&AsynTCPServerHandler::mainloop, this);
	m_thread = std::move(std::thread(l_threadTask));
	return true;
}

void AsynTCPServerHandler::shutdownTCPServer()
{
	LOG(INFO) << "AsynTCPServerHandler::shutdownTCPServer";
	m_threadExitFlag = true;
	try
	{
		m_thread.join();
	}
	catch(std::exception& e)
	{
		LOG(INFO) << "AsynUDPServerHandler::shutdownUDPServer catch exception, " << e.what();
	}
	m_clientInfoMutex.lock();
	for(auto l_cli = m_clientInfoMap.begin(); l_cli != m_clientInfoMap.end(); l_cli++)
	{
		if(l_cli->first > 0)
		{
			close(l_cli->first);
		}
	}
	m_clientInfoMutex.unlock();
	m_clientInfoMap.clear();
}

bool AsynTCPServerHandler::addClient(std::shared_ptr<ClientConnectInfo> p_cli)
{
	m_clientInfoMutex.lock();
	m_clientReadyToAdd.push_back(p_cli);
	m_clientInfoMutex.unlock();
	LOG(INFO) << "add client " << p_cli;
	return true;
}

bool AsynTCPServerHandler::delClient(int p_fd)
{
	m_clientInfoMutex.lock();
	m_clientReadyToDel.push_back(p_fd);
	m_clientInfoMutex.unlock();
	return true;
}

void AsynTCPServerHandler::updateClient()
{
	m_clientInfoMutex.lock();
	if(m_clientReadyToDel.empty() == false)
	{
		for(auto l_err : m_clientReadyToDel)
		{
			m_closecallback(m_clientInfoMap[l_err]);
			close(l_err);
			m_clientInfoMap.erase(l_err);
		}
	}
	m_clientReadyToDel.clear();

	if(m_clientReadyToAdd.empty() == false)
	{
		for(auto i = 0; i < m_clientReadyToAdd.size() ; i++)
		{
			m_clientInfoMap.erase(m_clientReadyToAdd[i]->m_sockfd);
			m_clientInfoMap.insert({m_clientReadyToAdd[i]->m_sockfd, m_clientReadyToAdd[i]});
		}
		m_clientReadyToAdd.clear();
	}
	m_clientInfoMutex.unlock();
}

bool AsynTCPServerHandler::readDataFromClient(int p_sockfd)
{
	char readbuffer[1024 * 8] = { 0 };
	memset(readbuffer, 0, sizeof(readbuffer));
	ssize_t l_nready = recv(p_sockfd, readbuffer, sizeof(readbuffer)-1, 0);
	if(l_nready <= 0)
	{
		LOG(INFO) << "AsynTCPServerHandler::readDataFromClient recv failed "
				<< ", " << strerror(errno);
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

		auto l_data = std::make_shared<EventTypeTCPClientDataObject>(
				p_sockfd, l_peerHost, l_peerPort, std::string(readbuffer));
		m_datacallback(l_data);
		return true;
	}
}

void AsynTCPServerHandler::mainloop()
{
	LOG(INFO) << "AsynTCPServerHandler::mainloop start";
	struct pollfd* fds = NULL;
	int l_fdsbufferSize = 0;

	while(m_threadExitFlag == false)
	{
		updateClient();

		if(m_clientInfoMap.empty() == true)
		{
			sleep(1);
			continue;
		}

		int l_clientSize = m_clientInfoMap.size();
		if(l_clientSize > l_fdsbufferSize)
		{
			fds = (struct pollfd*)realloc(fds, sizeof(struct pollfd) * l_clientSize);
			if(fds == NULL)
			{
				LOG(INFO) << "AsynTCPServerHandler::mainloop - realloc failed";
				continue;
			}
			l_fdsbufferSize = l_clientSize;
		}
		memset(fds, 0, sizeof(struct pollfd) * l_fdsbufferSize);

		int i = 0;
		for(auto l_cli = m_clientInfoMap.begin(); l_cli != m_clientInfoMap.end(); l_cli++, i++)
		{
			fds[i].fd = l_cli->first;
			fds[i].events = POLLRDNORM;
		}

		int nready = poll(fds, l_clientSize, 1000);
		if(nready <= 0)
		{
			// poll error or timeout
		}
		else
		{
			for(auto i = 0; i < l_clientSize ; i++)
			{
				if(fds[i].revents & POLLERR)
				{
					LOG(INFO) << "AsynTCPServerHandler::mainloop - " << fds[i].fd << " is POLLERR";
					delClient(fds[i].fd);
					continue;
				}
				if(fds[i].revents & POLLRDNORM)
				{
					if(false == readDataFromClient(fds[i].fd))
					{
						LOG(INFO) << "AsynTCPServerHandler::mainloop - readDataFromClient failed";
						delClient(fds[i].fd);
					}
				}
			}
		}
	}

	if(fds != NULL)
	{
		free(fds);
		fds = NULL;
	}

	LOG(INFO) << "AsynTCPServerHandler::mainloop exit";
}

ssize_t AsynTCPServerHandler::writeTCPServerString(std::shared_ptr<EventTypeTCPClientDataObject> p_msg)
{
	int l_sendSize = send(p_msg->m_socketfd, p_msg->m_rawData.c_str(), p_msg->m_rawData.size(), 0);
	if(l_sendSize < 0)
	{
		LOG(INFO) << "AsynTCPServerHandler::writeTCPServerString send failed"
				<< ", " << strerror(errno);
	}
	return l_sendSize;
}

std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeTCPClientDataObject> obj)
{
	os << "Type:Andlink Device Plugin";
	os << ", fd:" << obj->m_socketfd;
	os << ", size:" << obj->m_rawData.size();
	os << ", IP:" << obj->m_host;
	os << ", Port:" << obj->m_port;
	os << ", msg: " << obj->m_rawData;
	return os;
}

