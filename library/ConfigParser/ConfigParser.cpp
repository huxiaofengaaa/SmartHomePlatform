#include "ConfigParser.hpp"

ConfigParser::ConfigParser(std::string p_filename):
	m_fileReader(std::ifstream(p_filename, std::ios::in)),
	m_vaild(false)
{
	m_vaild = parser();
}

ConfigParser::~ConfigParser()
{

}

bool ConfigParser::isVaild()
{
	return m_vaild;
}

std::string ConfigParser::getParamString(std::string p_name, std::string p_default)
{
	auto l_search = m_paramList.find(p_name);
	if(l_search == m_paramList.end())
	{
		return p_default;
	}
	return l_search->second;
}

int ConfigParser::getParamInteger(std::string p_name, int p_default)
{
	auto l_search = m_paramList.find(p_name);
	if(l_search == m_paramList.end())
	{
		return p_default;
	}
	int l_value = p_default;
	try
	{
		l_value = std::stoi(l_search->second);
	}
	catch(...)
	{

	}
	return l_value;
}

bool ConfigParser::parser()
{
	if(m_fileReader.is_open() == false)
	{
		return false;
	}
	char buffer[1024] = { 0 };
	while(true)
	{
		memset(buffer, 0, sizeof(buffer));
		m_fileReader.getline(buffer, sizeof(buffer)-1);
		if(m_fileReader.gcount() <= 0)
		{
			break;
		}
		if(false == parserLine(std::string(buffer)))
		{
			continue;
		}
	}
	return true;
}

void ConfigParser::show()
{
	for(auto i = m_paramList.begin() ; i != m_paramList.end() ; i++)
	{
		std::cout << i->first << ":" << i->second << std::endl;
	}
}

std::string ConfigParser::removeSpaceFromBothEnd(std::string p_str)
{
	bool l_startFlag = false;
	std::string::size_type l_start = 0;
	std::string::size_type l_end = p_str.size();

	int i = 0;
	while(p_str[i] == ' ' || p_str[i] == '\t')
	{
		i++;
	}
	l_start = i;

	i = p_str.size() - 1;
	while(p_str[i] == ' ' || p_str[i] == '\t')
	{
		i--;
	}
	l_end = i + 1;

	return p_str.substr(l_start, l_end - l_start);
}

bool ConfigParser::parserLine(std::string p_line)
{
	std::string::size_type l_position = p_line.find("=");
	if(l_position == std::string::npos)
	{
		return false;
	}
	std::string l_key = removeSpaceFromBothEnd(p_line.substr(0, l_position));
	std::string l_value = removeSpaceFromBothEnd(p_line.substr(l_position+1, p_line.size() - l_position - 1));
	if(l_key.empty() == false && l_value.empty() == false)
	{
		m_paramList.emplace(std::make_pair(l_key, l_value));
	}
	return true;
}
