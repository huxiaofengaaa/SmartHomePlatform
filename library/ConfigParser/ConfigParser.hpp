#ifndef LIBRARY_CONFIGPARSER_CONFIGPARSER_HPP_
#define LIBRARY_CONFIGPARSER_CONFIGPARSER_HPP_

#include <string.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

class ConfigParser
{
public:
	ConfigParser(std::string p_filename = "config.ini");
	~ConfigParser();
	bool isVaild();
	std::string getParamString(std::string p_name, std::string p_default);
	int getParamInteger(std::string p_name, int p_default);
	void show();
	std::pair<std::string, std::string> operator[](int index);
	int size() const;
private:
	bool parser();
	std::string removeSpaceFromBothEnd(std::string p_str);
	bool parserLine(std::string p_line);

	bool m_vaild;
	FILE* m_fileReader;
	std::map<std::string, std::string> m_paramList;
};

#endif /* LIBRARY_CONFIGPARSER_CONFIGPARSER_HPP_ */
