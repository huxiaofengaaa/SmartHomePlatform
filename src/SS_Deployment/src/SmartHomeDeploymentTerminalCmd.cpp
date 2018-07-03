/*
 * SmartHomeDeploymentTerminalCmd.cpp
 *
 *  Created on: Jul 3, 2018
 *      Author: xiaofenh
 */

#include "SmartHomeDeployment.hpp"

std::string SmartHomeDeployment::terminalCmdHelp()
{
	return m_terminalExcutionUnit->getAllTerminalCmdUsage();
}


