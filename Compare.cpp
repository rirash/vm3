#include <stdexcept>
#include <iostream>
#include "Compare.h"

void Cmp::change_flag(float res, Processor& cpu)
{
	if (res < 0) cpu.psw.setSF(1);
	else cpu.psw.setSF(0);
	if (fabs(res) < eps) cpu.psw.setZF(1);
	else cpu.psw.setZF(0);
}

void Cmp::operator()(Processor& cpu)
{
	cmd32 com = cpu.get_Command32();
	uint16_t adr1 = 0, adr2 = 0;
	switch (com.bb)
	{
	case 10:
	{
		adr1 = cpu.reg[0] + com.off1;
		adr2 = cpu.reg[1];
		break;
	}
	case 01:
	{
		adr1 = cpu.reg[0];
		adr2 = cpu.reg[1] + com.off2;
		break;
	}
	case 11:
	{
		adr1 = cpu.reg[0] + com.off1;
		adr2 = cpu.reg[1] + com.off2;
		break;
	}
	}	

	datatype32 op1 = cpu.memory[adr1];
	datatype32 op2 = cpu.memory[adr2];
	float res = op1.w32.i32 - op2.w32.i32;
	change_flag(res, cpu);
}