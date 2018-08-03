#ifndef SRC_SS_UECONTEXT_INCLUDE_DATABASESTORAGE_HPP_
#define SRC_SS_UECONTEXT_INCLUDE_DATABASESTORAGE_HPP_

#include <string>
#include <stdio.h>
#include "hiredis.h"
#include "glog/logging.h"

class DataBaseStorage
{
public:
	DataBaseStorage(std::string p_redisHost = "127.0.0.1",
			int p_redisPort = 6379, std::string p_redisAuthPwd = std::string());
	virtual ~DataBaseStorage();
	bool statup(std::string p_redisHost, int p_redisPort, std::string p_redisAuthPwd);
	bool reStartup(std::string p_redisHost, int p_redisPort, std::string p_redisAuthPwd);
	bool shutdown();

private:
	bool connectRedis();
	bool authRedis();
	bool disconnectRedis();

	redisContext *m_redisClientHandler;
	bool m_connectToRemoteFlag;
	std::string m_redisHost;
	int m_redisPort;
	std::string m_redisAuthPwd;
};

#endif /* SRC_SS_UECONTEXT_INCLUDE_DATABASESTORAGE_HPP_ */
