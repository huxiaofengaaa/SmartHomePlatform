/*
 * UDPClientHandler.hpp
 *
 *  Created on: 2018Äê7ÔÂ17ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_SS_SYSMECHANISH_INCLUDE_UDPCLIENTHANDLER_HPP_
#define SRC_SS_SYSMECHANISH_INCLUDE_UDPCLIENTHANDLER_HPP_

#include <string>
#include <thread>
#include <functional>
#include <semaphore.h>
#include <mutex>

class UDPClientHandler
{
public:
	UDPClientHandler(std::string p_host, int p_port, std::function<bool(std::string)> p_callback);
	virtual ~UDPClientHandler();
	ssize_t writeUDPServerString(std::string p_data);
	std::string writeUDPServerAndGetResponse(std::string p_data);
	bool runUDPClient();
	void shutDownUDPClient();
private:
	int createUDPClientSocket(std::string p_host, int p_port);
	void mainloop();
	bool readDataFromServer(int p_sockfd);
	void setNeedResponseFlag();
	void reSetNeedResponseFlag();

	sem_t m_getResponse_sem;
	std::string m_response;
	bool m_needResponse;
	std::mutex m_responseMutex;

	const std::string m_host;
	const int m_port;
	int m_sockfd;

	std::thread m_thread;
	std::function<bool(std::string)> m_callback;
	bool m_threadExitFlag;
};

#endif /* SRC_SS_SYSMECHANISH_INCLUDE_UDPCLIENTHANDLER_HPP_ */
