/*
 * PacketStatistician.hpp
 *
 *  Created on: 2018Äê7ÔÂ10ÈÕ
 *      Author: Administrator
 */
#pragma once
#include <mutex>
#include "glog/logging.h"

class PacketStatistician
{
public:
	PacketStatistician():
		m_sendPackets(0),m_recvPackets(0),m_sendBytes(0),m_recvBytes(0)
	{

	}
	virtual ~PacketStatistician()
	{

	}
	int getSendPacket()
	{
		int l_number = 0;
		m_mutex.lock();
		l_number = m_sendPackets;
		m_mutex.unlock();
		return l_number;
	}
	int getRecvPacket()
	{
		int l_number = 0;
		m_mutex.lock();
		l_number = m_recvPackets;
		m_mutex.unlock();
		return l_number;
	}
	int getSendBytes()
	{
		int l_number = 0;
		m_mutex.lock();
		l_number = m_sendBytes;
		m_mutex.unlock();
		return l_number;
	}
	int getRecvBytes()
	{
		int l_number = 0;
		m_mutex.lock();
		l_number = m_recvBytes;
		m_mutex.unlock();
		return l_number;
	}
	void countSendPacket(int p_size)
	{
		m_mutex.lock();
		m_sendPackets += 1;
		m_sendBytes += p_size;
		m_mutex.unlock();
	}
	void countRecvPacket(int p_size)
	{
		m_mutex.lock();
		m_recvPackets += 1;
		m_recvBytes += p_size;
		m_mutex.unlock();
	}
private:
	int m_sendPackets;
	int m_recvPackets;

	int m_sendBytes;
	int m_recvBytes;

	std::mutex m_mutex;
};
