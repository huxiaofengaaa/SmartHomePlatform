#ifndef CLIENT_INCLUDE_UDPCLIENT_HPP_
#define CLIENT_INCLUDE_UDPCLIENT_HPP_

#include <string>

class UDPClient
{
public:
	UDPClient(std::string p_host, int p_port);
	virtual ~UDPClient();
	bool startUDPClient();
	void shutDownUDPClient();
private:
	int create_an_udp_socket_client(std::string p_host, int p_port);

	const std::string m_host;
	const int m_port;
	int m_sockfd;
};

#endif /* CLIENT_INCLUDE_UDPCLIENT_HPP_ */
