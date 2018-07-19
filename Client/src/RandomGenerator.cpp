#include "RandomGenerator.hpp"
#include <random>
#include <ctime>
#include <cstdlib>

std::string RandomGenerator::generatorRandomCharString(int length)
{
#ifdef CROSS_BUILD
	std::srand(std::time(NULL));
	char* buf = new char[length];

	for (int n = 0; n < length; ++n)
	{
		buf[n] = std::rand() % ('Z'-'A') + 'A';
	}
	buf[length] = '\0';

	std::string l_result = buf;
	delete[] buf;
	return l_result;
#else
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 'Z'-'A');

	char* buf = new char[length];

	for (int n = 0; n < length; ++n)
	{
		buf[n] = dis(gen) + 'A';
	}
	buf[length] = '\0';

	std::string l_result = buf;
	delete[] buf;
	return l_result;
#endif
}

std::string RandomGenerator::generatorRandomNumberString(int length)
{
#ifdef CROSS_BUILD
	std::srand(std::time(NULL));
	char* buf = new char[length];

	for (int n = 0; n < length; ++n)
	{
		buf[n] = std::rand() % 10 + '0';
	}
	buf[length] = '\0';

	std::string l_result = buf;
	delete[] buf;
	return l_result;
#else
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 9);

	char* buf = new char[length];

	for (int n = 0; n < length; ++n)
	{
		buf[n] = dis(gen) + '0';
	}
	buf[length] = '\0';

	std::string l_result = buf;
	delete[] buf;
	return l_result;
#endif
}

