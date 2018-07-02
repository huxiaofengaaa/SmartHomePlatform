#include "ContainerNetwork.hpp"
#include "SocketHandle.hpp"

int INetworkContainer::getSocketfd() const
{
	return m_sockfd;
}

void INetworkContainer::setSockfd(int p_socketfd)
{
	m_sockfd = p_socketfd;
}

std::string INetworkContainer::getHost() const
{
	return m_host;
}

int INetworkContainer::getPort() const
{
	return m_port;
}

bool UDPNetworkContainer::create()
{
	int l_udpSockfd = createUDPServerSocket(getHost(), getPort());
	if(l_udpSockfd > 0)
	{
		setSockfd(l_udpSockfd);
		return true;
	}
	return false;
}
std::shared_ptr<NetworkData> UDPNetworkContainer::read(int p_timeoutSec, int p_timeoutUsec)
{
	return readDataFromSocketFdWithTimeout(getSocketfd(), p_timeoutSec, p_timeoutUsec);
}


