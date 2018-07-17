#ifndef CLIENT_INCLUDE_TCPCLIENT_HPP_
#define CLIENT_INCLUDE_TCPCLIENT_HPP_

#include <string>

class TCPClient
{
public:
	TCPClient(std::string p_host, int p_port);
	virtual ~TCPClient();
	bool startTCPClient();
	void shutDownTCPClient();
private:
	int create_an_tcp_socket_client(std::string p_host,int p_port);

	const std::string m_host;
	const int m_port;
	int m_sockfd;
};

#endif /* CLIENT_INCLUDE_TCPCLIENT_HPP_ */
