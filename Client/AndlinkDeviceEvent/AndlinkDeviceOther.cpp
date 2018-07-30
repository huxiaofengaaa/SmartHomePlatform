#include "AndlinkDeviceEvent.hpp"

bool is_jSON_data(std::string msg)
{
	if(msg.empty() == false)
	{
		cJSON* obj = cJSON_Parse(msg.c_str());
		if(obj != NULL)
		{
			cJSON_Delete(obj);
			return true;
		}
	}
	return false;
}




