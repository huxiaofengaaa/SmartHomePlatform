#include "ExcutionUnitClient.hpp"
#include <unistd.h>
#include <stdio.h>
#include <iostream>

std::string g_serverHost = "127.0.0.1";
int g_serverPort = 6778;
//debug_mode,for rtl_link in realtek mode
int debug_mode = 2;

void showUsage()
{
	std::cout << "Usage: \n";
	std::cout << "    SmartHomeDevice [option]\n";
	std::cout << "    -h       show help document\n";
	std::cout << "    -H       set server host ip\n";
	std::cout << "    -P       ser server Port\n";
}

bool resolveInputOption(int argc, char** argv)
{
	int opt;
	while(true)
	{
		opt = getopt(argc, argv, "hH:P:");
		switch(opt)
		{
		case -1:
			return true;
		case 'h':
			showUsage();
			exit(0);
		case 'H':
			g_serverHost = optarg;
			printf("set server host %s\n", g_serverHost.c_str());
			break;
		case 'P':
			g_serverPort = atoi(optarg);
			printf("set server port %d \n", g_serverPort);
			break;
		default:
			printf("unknown option %c  %s\n", opt, optarg);
			break;
		}
	}
}

int main(int argc, char** argv)
{
	resolveInputOption(argc, argv);

	ExcutionUnitClient l_eu{ g_serverHost, g_serverPort};
	l_eu.startup(true);

	while(true)
	{
		sleep(1);
		printf("wait ExcutionUnitClient\n");
	}
	l_eu.shutdown();
	return 0;
}
