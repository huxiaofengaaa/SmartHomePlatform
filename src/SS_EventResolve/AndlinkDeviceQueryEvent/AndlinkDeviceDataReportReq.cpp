#include "AndlinkDeviceQueryEvent.hpp"

bool resolveAndlinkDeviceDataReportReq(std::string msg, struct Interface56_DataReport_Req* req)
{
	int i = 0;
	if(msg.empty() == true || req == NULL)
	{
		return false;
	}

	cJSON* obj = cJSON_Parse(msg.c_str());
	if(obj == NULL)
	{
		return false;
	}

	cJSON* data = cJSON_GetObjectItem(obj, "data");
	if(data == NULL)
	{
		cJSON_Delete(obj);
		return false;
	}

	cJSON* RPCMethod = cJSON_GetObjectItem(obj, "RPCMethod");
	cJSON* MAC = cJSON_GetObjectItem(obj, "MAC");
	cJSON* eventType = cJSON_GetObjectItem(obj, "eventType");
	cJSON* timestamp = cJSON_GetObjectItem(obj, "timestamp");
	cJSON* respCode = cJSON_GetObjectItem(data, "respCode");
	cJSON* restCont = cJSON_GetObjectItem(data, "restCont");

	if(RPCMethod && MAC && eventType && timestamp && respCode && restCont
			&& RPCMethod->valuestring == "data")
	{
		req->RPCMethod = RPCMethod->valuestring;
		req->MAC = MAC->valuestring;
		req->eventType = eventType->valuestring;
		req->timestamp = RPCMethod->valueint;
		req->respCode = respCode->valueint;
		req->restCont = restCont->valuestring;

		cJSON_Delete(obj);
		return true;
	}
	cJSON_Delete(obj);
	return false;
}

std::string buildAndlinkDeviceDataReportReq(struct Interface56_DataReport_Req req)
{
	std::string l_result;
	cJSON *regJs = cJSON_CreateObject();
	if(regJs == NULL)
	{
		return l_result;
	}

	cJSON *data = cJSON_CreateObject();
	if(data == NULL)
	{
		cJSON_Delete(regJs);
		return l_result;
	}
	cJSON_AddItemToObject(regJs, "data", data);

	cJSON_AddStringToObject(regJs, "RPCMethod", "data");
	cJSON_AddStringToObject(regJs, "MAC", req.MAC.c_str());
	cJSON_AddStringToObject(regJs, "eventType", req.eventType.c_str());
	cJSON_AddNumberToObject(regJs, "timestamp", req.timestamp);
	cJSON_AddNumberToObject(data, "respCode", req.respCode);
	cJSON_AddStringToObject(data, "restCont", req.restCont.c_str());

	char* regch = cJSON_Print(regJs);
	l_result = std::string(regch);
	cJSON_Delete(regJs);
	free(regch);

	return l_result;
}




