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
