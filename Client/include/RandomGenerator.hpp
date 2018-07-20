#pragma once
#include <string>

class RandomGenerator
{
public:
	std::string generatorRandomCharString(int length);
	std::string generatorRandomNumberString(int length);
	std::string generatorRandomMAC();
};
