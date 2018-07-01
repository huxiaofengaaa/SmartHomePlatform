#pragma once

#include <memory>
#include <string>
#include <string.h>

enum class ANDLINKDeviceDataType
{
	E_ANDLINK_DEVICE_REGISTER_REQ,
	E_ANDLINK_DEVICE_REGISTER_RESP,
	E_ANDLINK_DEVICE_ONLINE_REQ,
	E_ANDLINK_DEVICE_ONLINE_RESP,
	E_ANDLINK_DEVICE_AUTH_REQ,
	E_ANDLINK_DEVICE_AUTH_RESP,
	E_ANDLINK_DEVICE_HEARTBEAT_REQ,
	E_ANDLINK_DEVICE_HEARTBEAT_RESP,
	E_ANDLINK_DEVICE_REQUESTPLUGIN_REQ,
	E_ANDLINK_DEVICE_REQUESTPLUGIN_RESP
};

enum class EventType
{
	E_EVENT_TYPE_UNKNOWN,
	E_EVENT_TYPE_ANDLINK_DEVICE,
	E_EVENT_TYPE_TERMINAL_INPUT
};

struct EventTypeDataObject
{
	EventTypeDataObject(EventType p_type, const char* p_rawdata, int p_size);
	~EventTypeDataObject();

	EventType m_eventType;
	char* m_rawData;
};

