#include "UDPClient.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <arpa/inet.h>

UDPClient::UDPClient(std::string p_host, int p_port): m_host(p_host), m_port(p_port), m_sockfd(-1)
{

}

UDPClient::~UDPClient()
{

}

bool UDPClient::startUDPClient()
{
	int l_udpSocketfd = create_an_udp_socket_client(m_host, m_port);
	if(l_udpSocketfd < 0)
	{
		return false;
	}
	m_sockfd = l_udpSocketfd;
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



