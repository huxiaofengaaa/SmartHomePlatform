/*
 * 02_MD5File.cpp
 *
 *  Created on: 2018Äê7ÔÂ20ÈÕ
 *      Author: Administrator
 */
#include "MD5.h"
#include <iostream>

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		return -1;
	}
	char md5_str[64] = { 0 };
	Compute_file_md5(argv[1], md5_str);

	std::cout << md5_str << std::endl;
	return 0;
}



