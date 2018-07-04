#include "EventTypeStruct.hpp"

std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeTerminalDataObject> obj)
{
	switch(obj->m_eventType)
	{
	case EventType::E_EVENT_TYPE_UNKNOWN:
		os << "type:unknown";
		break;

	case EventType::E_EVENT_TYPE_ANDLINK_DEVICE:
		os << "type:andlink device";
		break;

	case EventType::E_EVENT_TYPE_TERMINAL_INPUT:
		os << "type:termnal input";
		break;
	default:
		break;
	}
	os << ", size: " << obj->m_rawData.size();
	os << ", msg: " << obj->m_rawData;

	return os;
}

std::ostream& operator<<(std::ostream& os, std::shared_ptr<EventTypeUDPClientDataObject> obj)
{
	switch(obj->m_eventType)
	{
	case EventType::E_EVENT_TYPE_UNKNOWN:
		os << "type:unknown";
		break;

	case EventType::E_EVENT_TYPE_ANDLINK_DEVICE:
		os << "type:andlink device";
		break;

	case EventType::E_EVENT_TYPE_TERMINAL_INPUT:
		os << "type:termnal input";
		break;
	default:
		break;
	}
	os << ", fd:" << obj->m_serverSocketFd;
	os << ", size:" << obj->m_rawData.size();
	os << ", IP:" << obj->m_host;
	os << ", Port:" << obj->m_port;
//	os << ", IP:" << inet_ntoa(obj->m_clientAddr.sin_addr);
//	os << ", Port:" << ntohs(obj->m_clientAddr.sin_port);
	os << ", msg: " << obj->m_rawData;
	return os;
}
