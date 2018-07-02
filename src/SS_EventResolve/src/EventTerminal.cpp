#include "EventTerminal.hpp"

TerminalCommnadHandler::TerminalCommnadHandler(std::shared_ptr<SmartHomeDeployment> p_deployment)
	: m_deployment(p_deployment)
{

}

TerminalCommnadHandler::~TerminalCommnadHandler()
{

}

bool TerminalCommnadHandler::registerCmd()
{

	return true;
}

bool TerminalCommnadHandler::deRegisterCmd()
{

	return true;
}

bool TerminalCommnadHandler::run(std::string p_cmd)
{

	return true;
}
