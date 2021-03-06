#include <memory>
#include "glog/logging.h"
#include "SmartHomeDeployment.hpp"
#include "ConfigParser.hpp"

int main(int argc, char** argv)
{
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "Welcome to SmartHomePlatform...";
	auto l_parser = std::make_shared<ConfigParser>();
	for(auto i = 0 ; i < l_parser->size() ; i++)
	{
		auto l_config = l_parser->operator[](i);
		LOG(INFO) << l_config.first << " = " << l_config.second;
	}
	//l_parser->show();
	auto l_deployment = std::make_shared<SmartHomeDeployment>(l_parser);
	l_deployment->start();
	LOG(INFO) << "Say Good Bye!!!";
	return 0;
}
