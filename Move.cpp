#include "Move.h"

void Move::operator()(Processor& cpu)
{
	process(cpu.get_Command24(), cpu);
}

void MoveR1::process(cmd24 com, Processor& cpu)
{
	cpu.reg[0] = com.adr;
}

void MoveR2::process(cmd24 com, Processor& cpu)
{
	cpu.reg[1] = com.adr;
}