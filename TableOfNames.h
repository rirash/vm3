#ifndef TABLE_OF_NAMES_H
#define TABLE_OF_NAMES_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream> 
#include <iostream>
#include "Types.h"
#include "TableOfCommands.h"


class TableOfNames
{
private:
	std::vector<std::string> splitStr(std::string inputStr, char delimiter)
	{
		std::vector <std::string> outputStr;		
		std::istringstream s(inputStr);
		std::istringstream input2;
		input2.str(inputStr);

		for (std::string line; std::getline(input2, line, delimiter); )
		{
			outputStr.push_back(line);
		}

		return outputStr;
	}

public:
	std::vector<elementOfNamesTable> _table;

	TableOfNames(std::string filePath, TableOfCommands tabOfCom)
	{
		std::ifstream reader(filePath);
		uint16_t idx = 0;
		std::string temp = "";
		//-----------------------.data----------------------------
		std::getline(reader, temp); //Пропустили первую строку
		std::getline(reader, temp);
		while (temp[0] != '.') //Пока не встретили .code
		{
			std::vector<std::string> splitTemp = splitStr(temp, ' ');
			word32 val;
			if (splitTemp[2] != "?")
			{
				if (splitTemp[1] == "I") val.i32 = std::stoi(splitTemp[2]);
				else val.f = std::stof(splitTemp[2]);
			}
			else
			{
				val.i32 = 0;
			}
			_table.push_back({ splitTemp[0], idx, val, false, splitTemp[1]});

			std::getline(reader, temp);
			idx += 4; // Т.к. размер операндов -- по 4 байта
		}

		//-----------------------.code----------------------------
		std::getline(reader, temp);
		while (!reader.eof())
		{
			std::vector<std::string> splitTemp = splitStr(temp, ' ');
			if (!tabOfCom.findCommand(splitTemp[0])) //Нашли метку
			{
				_table.push_back({ splitTemp[0], idx, 0, true, "not"});
			}
			else
			{
				idx += tabOfCom.getSize(splitTemp[0]);
			}
			std::getline(reader, temp);
		}

		reader.close();
	}

	uint16_t getAdr(std::string n)
	{
		return std::find_if(_table.begin(), _table.end(),
			[n](const elementOfNamesTable& item)
			{ return item.name == n; })->adr;
	}

	bool findName(std::string n)
	{
		std::vector<elementOfNamesTable>::iterator it =
			std::find_if(_table.begin(), _table.end(),
				[n](const elementOfNamesTable& item)
				{ return item.name == n; });
		return it != std::end(_table);
	}
};

#endif //!TABLE_OF_NAMES_H
