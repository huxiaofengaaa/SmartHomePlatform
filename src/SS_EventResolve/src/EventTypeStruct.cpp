#include "EventTypeStruct.hpp"

EventTypeDataObject::EventTypeDataObject(EventType p_type, const char* p_rawdata, int p_size)
{
	m_eventType = p_type;
	if(p_rawdata && p_size > 0)
	{
		m_rawData = new char[p_size];
		memcpy(m_rawData, p_rawdata, p_size);
	}
	else
	{
		m_eventType = EventType::E_EVENT_TYPE_UNKNOWN;
		m_rawData = NULL;

	}
}

EventTypeDataObject::~EventTypeDataObject()
{
	if(m_rawData)
	{
		delete m_rawData;
		m_rawData = NULL;
	}
}
