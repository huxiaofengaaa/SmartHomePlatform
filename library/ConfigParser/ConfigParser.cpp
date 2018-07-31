#include "ConfigParser.hpp"

ConfigParser::ConfigParser(std::string p_filename):
	m_fileReader(NULL),
	m_vaild(false)
{
	m_fileReader = fopen(p_filename.c_str(), "r");
	if(m_fileReader)
	{
		m_vaild = parser();
		fclose(m_fileReader);
		m_fileReader = NULL;
	}
	else
	{
		m_vaild = false;
	}
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
	if(m_vaild == true)
	{
		auto l_search = m_paramList.find(p_name);
		if(l_search == m_paramList.end())
		{
			return p_default;
		}
		return l_search->second;
	}
	return p_default;
}

int ConfigParser::getParamInteger(std::string p_name, int p_default)
{
	if(m_vaild == false)
	{
		return p_default;
	}
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
	char buffer[1024] = { 0 };
	while(true)
	{
		memset(buffer, 0, sizeof(buffer));
		char* l_result = fgets(buffer, sizeof(buffer) - 1, m_fileReader);
		if(l_result == NULL)
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
	if(m_vaild == true)
	{
		for(auto i = m_paramList.begin() ; i != m_paramList.end() ; i++)
		{
			std::cout << i->first << ":" << i->second << std::endl;
		}
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
	while(p_str[i] == ' ' || p_str[i] == '\t' || p_str[i] == '\n')
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

int ConfigParser::size() const
{
	return m_paramList.size();
}

std::pair<std::string, std::string> ConfigParser::operator[](int p_index)
{
	if(p_index >= 0 && p_index <= m_paramList.size() -1)
	{
		auto l_indexValue = m_paramList.begin();
		while(p_index-- && l_indexValue != m_paramList.end())
		{
			l_indexValue++;
		}
		return *l_indexValue;
	}
	return {};
}
