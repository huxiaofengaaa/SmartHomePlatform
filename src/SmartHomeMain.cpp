#include <memory>
#include "glog/logging.h"
#include "SmartHomeDeployment.hpp"

int main(int argc, char** argv)
{
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "Welcome to SmartHomePlatform...";
	auto l_deployment = std::make_shared<SmartHomeDeployment>();
	l_deployment->start();
	LOG(INFO) << "Say Good Bye!!!";
	return 0;
}
