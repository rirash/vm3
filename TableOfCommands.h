#ifndef TABLE_OF_COMMANDS_H
#define TABLE_OF_COMMANDS_H

#include <vector>
#include <algorithm>
#include "Types.h"

class TableOfCommands
{
private:
	std::vector<elementOfCommandsTable> _table;

public:
	TableOfCommands()
	{
		//Команда стоп
		_table.push_back({ "END", 0, 4 });

		//Пересылки
		_table.push_back({ "MOVadr_1", 1, 3 });
		_table.push_back({ "MOVadr_2", 2, 3 });
		_table.push_back({ "MOVm_1", 1, 3 });
		_table.push_back({ "MOVm_2", 2, 3 });

		//Целочисленная арифметика
		_table.push_back({ "ADDI", 3, 4 });
		_table.push_back({ "SUBI", 4, 4 });
		_table.push_back({ "MULI", 5, 4 });
		_table.push_back({ "DIVI", 6, 4 });
		_table.push_back({ "REM", 7, 4 });
		_table.push_back({ "INC", 8, 4 });
		_table.push_back({ "DEC", 9, 4 });
		_table.push_back({ "NEG", 10, 4 });

		//Битовые операции
		_table.push_back({ "AND", 11, 4 });
		_table.push_back({ "OR", 12, 4 });
		_table.push_back({ "XOR", 13, 4 });
		_table.push_back({ "NOT", 14, 4 });
		_table.push_back({ "SDVL", 15, 4 });
		_table.push_back({ "SDVR", 16, 4 });

		//Вещественная арифметика
		_table.push_back({ "ADDF", 17, 4 });
		_table.push_back({ "SUBF", 18, 4 });
		_table.push_back({ "MULF", 19, 4 });
		_table.push_back({ "DIVF", 20, 4 });
		_table.push_back({ "SQRT", 21, 4 });

		//Сравнения
		_table.push_back({ "CMP", 22, 4 });

		//Ввод+вывод
		_table.push_back({ "INI", 23, 3 });
		_table.push_back({ "INF", 24, 3 });
		_table.push_back({ "OUTI", 25, 3 });
		_table.push_back({ "OUTF", 26, 3 });

		//Переходы !!
		_table.push_back({ "JMP", 27, 2 });
		_table.push_back({ "JZF", 28, 2 });
		_table.push_back({ "JNZF", 29, 2 });
		_table.push_back({ "JSF", 30, 2 });
		_table.push_back({ "JNSF", 31, 2 });

		//Вызов к подпрограмме!!
		_table.push_back({ "PJMP", 32, 4 });
		_table.push_back({ "PJZF", 33, 4 });
		_table.push_back({ "PJNZF", 34, 4 });
		_table.push_back({ "PJSF", 35, 4 });
		_table.push_back({ "PJNSF", 36, 4 });

		_table.push_back({ "RET", 37, 4 });
	}

	uint8_t getMachineCode(std::string assemblerName)
	{
		return std::find_if(_table.begin(), _table.end(),
			[assemblerName](const elementOfCommandsTable& item)
			{ return item.assemblerName == assemblerName; })->machineCodeName;
	}

	uint8_t getSize(std::string assemblerName)
	{
		return std::find_if(_table.begin(), _table.end(),
			[assemblerName](const elementOfCommandsTable& item)
			{ return item.assemblerName == assemblerName; })->size;
	}

	bool findCommand(std::string assemblerName)
	{
		std::vector<elementOfCommandsTable>::iterator it =
			std::find_if(_table.begin(), _table.end(),
				[assemblerName](const elementOfCommandsTable& item)
				{ return item.assemblerName == assemblerName; });
		return it != std::end(_table);
	}
};

#endif //!TABLE_OF_COMMANDS_H
