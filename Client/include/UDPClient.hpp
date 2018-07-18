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
	int writeUDPString(std::string p_data);
	std::string readUDPString(int p_timeout);
	std::string writeAndReadUDPString(std::string p_data);

	std::string getLocalUDPHost();
	int getLocalUDPPort();

private:
	int create_an_udp_socket_client(std::string p_host, int p_port);

	const std::string m_host;
	const int m_port;
	int m_sockfd;

	std::string m_localhost;
	int m_localPort;
};

#endif /* CLIENT_INCLUDE_UDPCLIENT_HPP_ */
