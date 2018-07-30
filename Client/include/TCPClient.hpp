#ifndef CLIENT_INCLUDE_TCPCLIENT_HPP_
#define CLIENT_INCLUDE_TCPCLIENT_HPP_

#include <string>

class TCPClient
{
public:
	TCPClient(std::string p_host, int p_port);
	virtual ~TCPClient();
	bool startTCPClient(std::string p_host, int p_port);
	bool startTCPClient();
	void shutDownTCPClient();
	int writeTCPString(std::string p_data);
	std::string readTCPString(int p_timeout);
	std::string writeAndReadTCPString(std::string p_data);

	std::string getLocalTCPHost();
	int getLocalTCPPort();
private:
	int create_an_tcp_socket_client(std::string p_host,int p_port);

	std::string m_host;
	int m_port;
	int m_sockfd;

	std::string m_localhost;
	int m_localPort;
};

#endif /* CLIENT_INCLUDE_TCPCLIENT_HPP_ */
