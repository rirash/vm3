#ifndef TYPES_H
#define TYPES_H
#include<string>
#include <cstdint>

union word32
{
	int32_t i32;
	float f;
};

struct cmd32
{
	unsigned int KOP : 6;
	unsigned int bb : 2;
	unsigned int off1 : 12;
	unsigned int off2 : 12;
};

struct cmd24
{
	unsigned int KOP : 6;
	unsigned int bb : 2;
	unsigned int adr : 16;
};

struct cmd16
{
	unsigned int KOP : 6;
	unsigned int bb : 2;
	int off : 8;
};

union datatype24
{
	cmd24 cmd24;
	uint8_t byte[3];
};

union datatype16
{
	cmd16 cmd16;
	uint8_t byte[2];
};

union datatype32
{
	cmd32 cmd32;
	word32 w32;
	datatype16 dt16[2];
	uint8_t byte[4];
};

struct kopbb
{
	unsigned int KOP : 6;
	unsigned int bb : 2;
};

union datatype8
{
	kopbb kb;
	uint8_t byte;
};

union allCommand
{
	datatype32 dt32;
	datatype24 dt24;
};

struct elementOfNamesTable
{
	std::string name;
	uint16_t adr;
	word32 val;
	bool metka;
	std::string type;
};

struct elementOfCommandsTable
{
	std::string assemblerName;
	uint8_t machineCodeName;
	uint8_t size; //2, 3 или 4 байта
};

#endif //!TYPES_H