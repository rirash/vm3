#include <string>
#include <vector>
#include "TableOfNames.h"
#include "ReversePolishEntry.h"


std::vector<std::string> split(std::string inputStr, char delimiter)
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

std::string parseMATHAndCMPAndCallToMachineCode(std::string inputS, TableOfCommands& comTable, TableOfNames& nameTable, int& idxIP)
{
	std::string res = "";

	std::vector<std::string> splitInput = split(inputS, ' ');
	res += "c " + std::to_string(comTable.getMachineCode(splitInput[0]));

	if (splitInput[1][0] == '%') //КОП %r1 *смещение* , bb = 01
	{
		res += " 01 00 ";
		if (nameTable.findName(splitInput[2]))
		{
			res += std::to_string(nameTable.getAdr(splitInput[2]));
		}
		else
		{
			res += calcReversePolishEntry(splitInput[2]);
		}
	}
	else
	{
		if (splitInput[2][0] == '%') //КОП *смещение* %r2 , bb = 10
		{
			res += " 10 ";
			if (nameTable.findName(splitInput[1]))
			{
				res += std::to_string(nameTable.getAdr(splitInput[1]));
			}
			else
			{
				res += calcReversePolishEntry(splitInput[1]);
			}
			res += " 00";
		}
		else    //КОП *смещение* *смещение* , bb = 11
		{
			res += " 11 ";
			if (nameTable.findName(splitInput[1]))
			{
				res += std::to_string(nameTable.getAdr(splitInput[1]));
			}
			else
			{
				res += calcReversePolishEntry(splitInput[1]);
			}
			res += " ";
			if (nameTable.findName(splitInput[2]))
			{
				res += std::to_string(nameTable.getAdr(splitInput[2]));
			}
			else
			{
				res += calcReversePolishEntry(splitInput[2]);
			}
		}
	}

	idxIP += comTable.getSize(splitInput[0]);

	return res;
}

std::string parseMOVEAndInOutToMachineCode(std::string inputS, TableOfCommands& comTable, TableOfNames& nameTable, int& idxIP)
{
	std::string res = "";   // КОП bb adr
	std::vector<std::string> splitInput = split(inputS, ' ');
	res += "c " + std::to_string(comTable.getMachineCode(splitInput[0]));
	res += " 00 ";
	if (nameTable.findName(splitInput[1]))
	{
		res += std::to_string(nameTable.getAdr(splitInput[1]));
	}
	else
	{
		res += calcReversePolishEntry(splitInput[1]);
	}

	idxIP += comTable.getSize(splitInput[0]);

	return res;
}

std::string parseTransitionsToMachineCode(std::string inputS, TableOfCommands& comTable, TableOfNames& nameTable, int& idxIP)
{
	std::string res = ""; // КОП bb [r1/r2] adr
	std::vector<std::string> splitInput = split(inputS, ' ');
	res += "c " + std::to_string(comTable.getMachineCode(splitInput[0]));

	if (splitInput.size() == 2) // КОП bb [r1/r2] adr , bb = 00 , adr = IP + смещение
	{
		res += " 00 ";
		res += calcReversePolishEntry(splitInput[1]);
	}
	else
	{
		if (splitInput[1][2] == '1') //JMP %r1 13 , IP = *то, что хранится в r1* + 13)
		{
			res += " 10 ";
		}
		else   //JMP %r2 13 , IP = *то, что хранится в r2* + 13)
		{
			res += " 01 ";
		}

		res += calcReversePolishEntry(splitInput[2]);
	}

	idxIP += comTable.getSize(splitInput[0]);

	return res;
}

std::string parseReturnAndENDToMachineCode(std::string inputS, TableOfCommands& comTable, TableOfNames& nameTable, int& idxIP)
{
	std::string res = "";

	std::vector<std::string> splitInput = split(inputS, ' ');
	if (splitInput[0] == "END")
		res = "c 0 00 0 0";
	else
		res = "c 40 00 0 0";
	idxIP += 4;

	return res;
}

void translatorToMachineCode()
{
	int ipIdx = 0;
	TableOfCommands comTab;
	TableOfNames nameTab("Assembler.txt", comTab);

	std::ifstream reader("Assembler.txt");
	std::ofstream writer("MachineCode.txt");

	writer << "a 0\n";

	for (int i = 0; i < nameTab._table.size(); i++)  //.data
	{
		if (!nameTab._table[i].metka)
		{
			std::string output = nameTab._table[i].type + " ";
			if (nameTab._table[i].type == "I")
			{
				output += std::to_string(nameTab._table[i].val.i32);
			}
			else
			{
				output += std::to_string(nameTab._table[i].val.f);
			}

			ipIdx += 4;
			writer << output + "\n";
		}
	}
	std::string line = "";
	std::getline(reader, line);
	while (line != ".code")
	{
		std::getline(reader, line);
	}

	while (!reader.eof())   //.code
	{
		std::getline(reader, line);
		if (line != "")
		{
			std::vector<std::string> splitCommand = split(line, ' ');
			if (!nameTab.findName(splitCommand[0]))
			{
				uint8_t mCode = comTab.getMachineCode(splitCommand[0]);
				if ((mCode >= 3 && mCode <= 22) || (mCode >= 32 && mCode <= 36))
				{
					writer << parseMATHAndCMPAndCallToMachineCode(line, comTab, nameTab, ipIdx) << "\n";
				}

				if ((mCode == 1 || mCode == 2) || (mCode >= 23 && mCode <= 26))
				{
					writer << parseMOVEAndInOutToMachineCode(line, comTab, nameTab, ipIdx) << "\n";
				}

				if (mCode >= 27 && mCode <= 31)
				{
					writer << parseTransitionsToMachineCode(line, comTab, nameTab, ipIdx) << "\n";
				}

				if (mCode == 0 || mCode == 37)
				{
					writer << parseReturnAndENDToMachineCode(line, comTab, nameTab, ipIdx) << "\n";
				}
			}
		}
	}

	writer << "s 0\n";
	reader.close();
	writer.close();

}