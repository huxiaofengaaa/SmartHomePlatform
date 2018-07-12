/*
 * RandomGenerator.cpp
 *
 *  Created on: 2018Äê7ÔÂ12ÈÕ
 *      Author: Administrator
 */
#include "RandomGenerator.hpp"
#include <random>

std::string RandomGenerator::generatorRandomCharString(int length)
{
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
}

std::string RandomGenerator::generatorRandomNumberString(int length)
{
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
}

