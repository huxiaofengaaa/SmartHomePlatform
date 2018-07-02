#pragma once

#include <iostream>
#include <string>
#include "SocketHandle.hpp"

class INetworkContainer
{
public:
	INetworkContainer(std::string p_host, int p_port): m_host(p_host), m_port(p_port), m_sockfd(-1)
	{

	}
	virtual ~INetworkContainer()
	{

	}

	int getSocketfd() const;
	void setSockfd(int p_socketfd);
	std::string getHost() const;
	int getPort() const;

	virtual bool create() = 0;
	virtual std::shared_ptr<NetworkData> read(int p_timeoutSec, int p_timeoutUsec) = 0;

private:
	const std::string m_host;
	const int m_port;
	int m_sockfd;
};

class UDPNetworkContainer: public INetworkContainer
{
public:
	UDPNetworkContainer(std::string p_host = "", int p_port = 6887): INetworkContainer(p_host, p_port)
	{

	}
	~UDPNetworkContainer()
	{

	}
	bool create() override;
	std::shared_ptr<NetworkData> read(int p_timeoutSec, int p_timeoutUsec) override;

private:
};

class TCPNetworkContainer: public INetworkContainer
{
public:
	TCPNetworkContainer(std::string p_host = "", int p_port = 6888): INetworkContainer(p_host, p_port)
	{

	}
	~TCPNetworkContainer()
	{

	}
	bool create() override;
private:
};


