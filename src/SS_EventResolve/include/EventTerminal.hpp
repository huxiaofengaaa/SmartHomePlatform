#pragma once

#include <vector>
#include <memory>
#include <string>

class SmartHomeDeployment;

class TerminalCommand
{
public:
	TerminalCommand();
	~TerminalCommand();
private:

};

class TerminalCommnadHandler
{
public:
	TerminalCommnadHandler(std::shared_ptr<SmartHomeDeployment> p_deployment);
	~TerminalCommnadHandler();
	bool registerCmd();
	bool deRegisterCmd();
	bool run(std::string p_cmd);
private:

	std::shared_ptr<SmartHomeDeployment> m_deployment;
	std::vector<std::shared_ptr<TerminalCommand>> m_commandList;
};


