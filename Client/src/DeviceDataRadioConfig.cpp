#include "DeviceDataStore.hpp"
#include "DataBase.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

int DeviceRadioConfig::getRadioNumber()
{
	int l_radioNumber = 0;
#ifdef CROSS_BUILD
	l_radioNumber = get_radio_number();
#else

#endif
	return l_radioNumber;
}

std::tuple<std::string, int, std::string, int> DeviceRadioConfig::getRadioStatus(int p_index)
{
	char l_radioName[16] = { 0 };
	int l_radioEnable = 0;
	char l_radioPower[16] = { 0 };
	int l_radioChannel = 0;
#ifdef CROSS_BUILD
	get_radio_status(p_index, l_radioName, &l_radioEnable, l_radioPower, &l_radioChannel);
#else

#endif
	return std::tuple<std::string, int, std::string, int>(
			l_radioName, l_radioEnable, l_radioPower, l_radioChannel);
}

int DeviceRadioConfigurationList::getRadioConfigurationNumber()
{
	int l_radioConfigurationNumber = 0;
#ifdef CROSS_BUILD
	l_radioConfigurationNumber = get_radio_configuration_number();
#else
#endif
	return l_radioConfigurationNumber;
}

std::tuple<std::string, int, int, std::string, std::string, std::string, int, int>
DeviceRadioConfigurationList::getRadioConfigurationStatus(int p_index)
{
	char l_configurationName[16] = { 0 };
	int l_configurationIndex = p_index + 1;
	int l_configurationEnable = 0;
	char l_configurationSSID[32] = { 0 };
	char l_configurationSecurityMode[32] = { 0 };
	char l_configurationPwd[32] = { 0 };
	int l_configurationMaxAssociateNum = 0;
	int l_configurationSSIDAdvertisementEnabled = 0;
#ifdef CROSS_BUILD
	get_radio_configuration_status(
			p_index, l_configurationName, &l_configurationEnable, l_configurationSSID,
			l_configurationSecurityMode, l_configurationPwd, &l_configurationMaxAssociateNum,
			&l_configurationSSIDAdvertisementEnabled);
#else

#endif
	return std::tuple<std::string, int, int, std::string, std::string, std::string, int, int>(
			l_configurationName, l_configurationIndex, l_configurationEnable,
			l_configurationSSID, l_configurationSecurityMode, l_configurationPwd,
			l_configurationMaxAssociateNum, l_configurationSSIDAdvertisementEnabled);
}
