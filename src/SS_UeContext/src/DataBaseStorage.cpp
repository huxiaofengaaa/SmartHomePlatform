#include "DataBaseStorage.hpp"

DataBaseStorage::DataBaseStorage(std::string p_redisHost, int p_redisPort, std::string p_redisAuthPwd)
		: m_redisHost(p_redisHost),
		  m_redisPort(p_redisPort),
		  m_redisAuthPwd(p_redisAuthPwd),
		  m_connectToRemoteFlag(false),
		  m_redisClientHandler(NULL)
{
	LOG(INFO) << "default remote data base server is " << p_redisHost << ":" << m_redisPort;
}

DataBaseStorage::~DataBaseStorage()
{
	shutdown();
}

bool DataBaseStorage::statup(std::string p_redisHost, int p_redisPort, std::string p_redisAuthPwd)
{
	m_redisHost = p_redisHost;
	m_redisPort = p_redisPort;
	m_redisAuthPwd = p_redisAuthPwd;
	return connectRedis() && authRedis();
}

bool DataBaseStorage::reStartup(std::string p_redisHost, int p_redisPort, std::string p_redisAuthPwd)
{
	m_redisHost = p_redisHost;
	m_redisPort = p_redisPort;
	m_redisAuthPwd = p_redisAuthPwd;
	return disconnectRedis() && connectRedis() && authRedis();
}

bool DataBaseStorage::shutdown()
{
	return disconnectRedis();
}

bool DataBaseStorage::connectRedis()
{
	if(m_redisClientHandler != NULL)
	{
		LOG(INFO) << "already connect to redis, try disconnect";
		m_connectToRemoteFlag = true;
		return true;
	}

	struct timeval timeout = { 1, 500000 };
	m_redisClientHandler = redisConnectWithTimeout(m_redisHost.c_str(), m_redisPort, timeout);
	if(m_redisClientHandler == NULL || m_redisClientHandler->err)
	{
		if(m_redisClientHandler)
		{
			LOG(INFO) << "Redis server Connection error:" << m_redisClientHandler->errstr;
			redisFree(m_redisClientHandler);
			m_redisClientHandler = NULL;
		}
		else
		{
			LOG(INFO) << "Redis server Connection error: can't allocate redis context\n";
		}
		return false;
	}
	m_connectToRemoteFlag = true;
	LOG(INFO) << "connect to redis " << m_redisHost << ":" << m_redisPort << " successfully.";
	return true;
}

bool DataBaseStorage::disconnectRedis()
{
	if(m_redisClientHandler == NULL)
	{
		LOG(INFO) << "m_redisClientHandler not allocate, ignore disconnect request.";
		m_connectToRemoteFlag = false;
		return true;
	}

	redisFree(m_redisClientHandler);
	m_redisClientHandler = NULL;
	m_connectToRemoteFlag = false;
	LOG(INFO) << "disconnect to redis " << m_redisHost << ":" << m_redisPort;
	return true;
}

bool DataBaseStorage::authRedis()
{
	if(m_redisAuthPwd.empty() == true)
	{
		LOG(INFO) << "not set redis server password, ignore auth request";
		return true;
	}

	if(m_redisClientHandler == NULL)
	{
		LOG(INFO) << "m_redisClientHandler not allocate, auth failed.";
		return false;
	}

	bool l_authResult = true;
	redisReply *l_redisResp= (redisReply *)redisCommand(m_redisClientHandler,
			"auth %s", m_redisAuthPwd.c_str());
	if(l_redisResp != NULL && l_redisResp->type == 5 && l_redisResp->str == std::string("OK"))
	{
		LOG(INFO) << "Redis server Auth sucessfully";
		l_authResult = true;
	}
	else
	{
		LOG(INFO) << "Redis server Auth error, used password is " << m_redisAuthPwd;
		l_authResult = false;
	}
	if(l_redisResp)
	{
		freeReplyObject(l_redisResp);
	}
	return l_authResult;
}

bool DataBaseStorage::writeString(std::string p_key, std::string p_value)
{
	bool l_writeResult = false;
	if(m_redisClientHandler == NULL || m_connectToRemoteFlag == false)
	{
		LOG(INFO) << "write string failed, not connect to remote redis server";
		return l_writeResult;
	}

	redisReply *l_redisResp= (redisReply *)redisCommand(m_redisClientHandler,
			"set %s %s", p_key.c_str(), p_value.c_str());
	if(l_redisResp != NULL && l_redisResp->type == REDIS_REPLY_STATUS
			&& l_redisResp->str == std::string("OK"))
	{
		l_writeResult = true;
	}
	else
	{
		LOG(INFO) << "Redis write string error, key = " << p_key << " value = " << p_value;
		l_writeResult = false;
	}
	if(l_redisResp)
	{
		freeReplyObject(l_redisResp);
		l_redisResp = NULL;
	}
	return l_writeResult;
}

std::string DataBaseStorage::readString(std::string p_key, std::string p_defaultValue)
{
	std::string l_readResult = p_defaultValue;
	if(m_redisClientHandler == NULL || m_connectToRemoteFlag == false)
	{
		LOG(INFO) << "write string failed, not connect to remote redis server";
		return l_readResult;
	}

	redisReply *l_redisResp= (redisReply *)redisCommand(m_redisClientHandler, "get %s", p_key.c_str());
	if(l_redisResp != NULL && l_redisResp->type == REDIS_REPLY_STRING)
	{
		l_readResult = l_redisResp->str;
	}
	else
	{
		LOG(INFO) << "Redis read string error, key = " << p_key;
	}
	if(l_redisResp)
	{
		freeReplyObject(l_redisResp);
		l_redisResp = NULL;
	}
	return l_readResult;
}

bool DataBaseStorage::hashWriteOneField(std::string p_key, std::string p_field, std::string p_value)
{
	bool l_writeResult = false;
	if(m_redisClientHandler == NULL || m_connectToRemoteFlag == false)
	{
		LOG(INFO) << "redis hash write failed, not connect to remote redis server";
		return l_writeResult;
	}

	redisReply *l_redisResp= (redisReply *)redisCommand(m_redisClientHandler,
			"hset %s %s %s", p_key.c_str(), p_field.c_str(), p_value.c_str());
	if(l_redisResp != NULL && l_redisResp->type == REDIS_REPLY_INTEGER
			&& l_redisResp->integer == 0)
	{
		l_writeResult = true;
	}
	else
	{
		LOG(INFO) << "Redis hash write error, key = " << p_key
				  << " filed = " << p_field << " value = " << p_value;
		l_writeResult = false;
	}
	if(l_redisResp)
	{
		freeReplyObject(l_redisResp);
		l_redisResp = NULL;
	}
	return l_writeResult;
}

std::string DataBaseStorage::hashReadOneField(std::string p_key, std::string p_field, std::string p_defaultValue)
{
	std::string l_readResult = p_defaultValue;
	if(m_redisClientHandler == NULL || m_connectToRemoteFlag == false)
	{
		LOG(INFO) << "redis read hash failed, not connect to remote redis server";
		return l_readResult;
	}

	redisReply *l_redisResp= (redisReply *)redisCommand(m_redisClientHandler, "hget %s %s",
			p_key.c_str(), p_field.c_str());
	if(l_redisResp != NULL && l_redisResp->type == REDIS_REPLY_STRING)
	{
		l_readResult = l_redisResp->str;
	}
	else
	{
		LOG(INFO) << "Redis hash read error, key = " << p_key << " field = " << p_field;
	}
	if(l_redisResp)
	{
		freeReplyObject(l_redisResp);
		l_redisResp = NULL;
	}
	return l_readResult;
}

bool DataBaseStorage::setAdd(std::string p_key, std::string p_value)
{
	bool l_addResult = false;
	if(m_redisClientHandler == NULL || m_connectToRemoteFlag == false)
	{
		LOG(INFO) << "redis read set failed, not connect to remote redis server";
		return l_addResult;
	}
	redisReply *l_redisResp= (redisReply *)redisCommand(m_redisClientHandler, "sadd %s %s",
			p_key.c_str(), p_value.c_str());
	if(l_redisResp != NULL && l_redisResp->type == REDIS_REPLY_INTEGER)
	{
		if(l_redisResp->integer == 0)
		{
			LOG(INFO) << "set value " << p_value << " already exist in " << p_key;
		}
		l_addResult = true;
	}
	else
	{
		LOG(INFO) << "Redis set write error, key = " << p_key << " value = " << p_value;
		l_addResult = false;
	}
	if(l_redisResp)
	{
		freeReplyObject(l_redisResp);
		l_redisResp = NULL;
	}
	return l_addResult;

}

std::vector<std::string> DataBaseStorage::setGetAll(std::string p_key)
{
	std::vector<std::string> l_readResult;
	if(m_redisClientHandler == NULL || m_connectToRemoteFlag == false)
	{
		LOG(INFO) << "redis read set failed, not connect to remote redis server";
		return l_readResult;
	}

	redisReply *l_redisResp= (redisReply *)redisCommand(m_redisClientHandler, "smembers %s", p_key.c_str());
	if(l_redisResp != NULL && l_redisResp->type == REDIS_REPLY_ARRAY && NULL != l_redisResp->element)
	{
		for(int i = 0; i< l_redisResp->elements ; i++)
		{
			if(l_redisResp->element[i] != NULL)
			{
				l_readResult.push_back(l_redisResp->element[i]->str);
			}
		}
	}
	else
	{
		LOG(INFO) << "Redis set read error, key = " << p_key ;
	}
	if(l_redisResp)
	{
		freeReplyObject(l_redisResp);
		l_redisResp = NULL;
	}
	return l_readResult;
}

