#include "ExcutionUnitTerminal.hpp"

#define PLATFORM_NAME	"SmartHomePlatform # "

ExcutionUnitTerminal::ExcutionUnitTerminal(
		std::shared_ptr<UeContextHolderAndlink> p_ueContextHolder,
		std::shared_ptr<ExcutionUnitAndlink> p_euAndlink,
		std::shared_ptr<ExcutionUnitAndlinkPlugIn> p_euAndlinkPlugin):
	ExcutionUnitWithSignalQueue(
			"Terminal",
			1,
			std::bind(&ExcutionUnitTerminal::handleDataObject,
					this,
					std::placeholders::_1)),
	AsynTerminalHandler(std::bind(&ExcutionUnitTerminal::terminalAsycDataCallback,
			this,
			std::placeholders::_1)),
	m_ueContextHolder(p_ueContextHolder),
	m_euAndlink(p_euAndlink),
	m_euAndlinkPlugin(p_euAndlinkPlugin)
{
	LOG(INFO) << "construct ExcutionUnitTerminal";
}

ExcutionUnitTerminal::~ExcutionUnitTerminal()
{
	LOG(INFO) << "de-construct ExcutionUnitTerminal";
}

bool ExcutionUnitTerminal::start()
{
	LOG(INFO) << "ExcutionUnitTerminal start";
	runTerminal();
	startExcutionUnit();
	registerAllCmd();
	writeTerminalString(PLATFORM_NAME);
	return true;
}
void ExcutionUnitTerminal::shutdown()
{
	shutdownTerminal();
	shutdownExcutionUnit();
	LOG(INFO) << "ExcutionUnitTerminal shutdown";
}

bool ExcutionUnitTerminal::terminalAsycDataCallback(std::string p_data)
{
	countRecvPacket(p_data.size());
	return addDataObject(p_data);
}

bool ExcutionUnitTerminal::handleDataObject(std::string p_str)
{
	if(p_str.empty() == true)
	{
		return false;
	}

	if(p_str != std::string("\n"))
	{
		LOG(INFO) << "Terminal input, size:" << p_str.size() << ", msg:" << p_str;
		std::string l_result = runTerminalCmd(p_str);
		if(l_result.empty() == false)
		{
			countSendPacket(l_result.size());
			writeTerminalString(l_result);
		}
	}
	writeTerminalString(PLATFORM_NAME);
	return true;
}

std::string ExcutionUnitTerminal::runTerminalCmd(std::string p_cmd)
{
	std::string l_cmdName;
	for(auto i = 0; i < p_cmd.size(); i++)
	{
		if(p_cmd[i] == ' ' || p_cmd[i] == '\n')
		{
			l_cmdName = p_cmd.substr(0, i);
			break;
		}
	}

	auto l_cmdObj = m_cmdList.find(l_cmdName);
	if(l_cmdObj != m_cmdList.end())
	{
		return l_cmdObj->second->run(p_cmd);
	}
	else
	{
		return std::string("Unknow Terminal Cmd ") + p_cmd + terminalCmdCallback_help(p_cmd);
	}
}

void ExcutionUnitTerminal::registerCmd(std::string p_cmdName, std::shared_ptr<TerminalCmd> p_cmdObj)
{
	m_cmdList.erase(p_cmdName);
	m_cmdList.insert({p_cmdName, p_cmdObj});
}

void ExcutionUnitTerminal::registerAllCmd()
{
	registerCmd("help", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_help,
						this,
						std::placeholders::_1),
				"[help                    ], show all terminal cmd"));

	registerCmd("list", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_list,
						this,
						std::placeholders::_1),
				"[list                    ], list device information cmd"));

	registerCmd("statistics", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_statistics,
						this,
						std::placeholders::_1),
				"[statistics              ], show packet send and recv"));

	registerCmd("plugin", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_plugin,
						this,
						std::placeholders::_1),
				"[plugin deviceID         ], trigger andlink device connect to TCP server"));

	registerCmd("disconnect", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_disconnect,
						this,
						std::placeholders::_1),
				"[disconnect deviceID     ], trigger andlink device disconnect to TCP server"));

	registerCmd("query", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_query,
						this,
						std::placeholders::_1),
				"[query deviceID paramName], query device parameter(apinfo/stainfo/upsts/wifists)."));

	registerCmd("reboot", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				"[reboot deviceID         ], restart device from remote"));

	registerCmd("led", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				"[led deviceID on/off     ], turn on/off led from remote"));

	registerCmd("power", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				std::string("[power deviceID radio val], set radio(0->all 1->2.4G other->5G) " +
						std::string("power(0~100 percent) from remote"))));

	registerCmd("macfilter", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				"[macfilter deviceID enable poligy entry], set device mac filter from remote"));

	registerCmd("chReselct", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				"[chReselct deviceID select], set channel radio(0->all 1->2.4G other->5G) reselect from remote"));

	registerCmd("roaming", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				"[roaming deviceID enable val1 val2], set device roaming from remote"));

	registerCmd("wifisync", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				"[wifisync deviceID], sync device wifi parameter from remote"));

	registerCmd("wifiswitch", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				"[wifiswitch deviceID enable on/off], turn on/off wifi from remote"));

	registerCmd("wps", std::make_shared<TerminalCmdNormal>(
				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
						this,
						std::placeholders::_1),
				"[wps deviceID on/off], turn on/off wps from remote"));
//
//	registerCmd("unbind", std::make_shared<TerminalCmdNormal>(
//				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
//						this,
//						std::placeholders::_1),
//				"[unbind deviceID], unbind device from remote"));
//
//	registerCmd("upgrade", std::make_shared<TerminalCmdNormal>(
//				std::bind(&ExcutionUnitTerminal::terminalCmdCallback_control,
//						this,
//						std::placeholders::_1),
//				"[upgrade deviceID pathname], upgrade device from remote"));
}

