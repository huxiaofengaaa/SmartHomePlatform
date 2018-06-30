#include <memory>
#include "SmartHomeDeployment.hpp"

int main(int argc, char** argv)
{
	auto l_deployment = std::make_shared<SmartHomeDeployment>();
	l_deployment->start();
	l_deployment->shutdown();
	return 0;
}
