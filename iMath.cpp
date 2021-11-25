#include "iMath.h"

void iMath::change_flag(int32_t res, Processor& cpu) noexcept
{
	if (res < 0) cpu.psw.setSF(1);
	else cpu.psw.setSF(0);
	if (res == 0) cpu.psw.setZF(1);
	else cpu.psw.setZF(0);
}

void iMath::operator()(Processor& cpu)
{
	cmd32 com = cpu.get_Command32();
	uint16_t adr1 = 0, adr2 = 0;
	switch (com.bb)
	{
	case 01:
	{
		uint16_t adr1 = cpu.reg[0];
		uint16_t adr2 = cpu.reg[1] + com.off2;
		break;
	}
	case 10:
	{
		uint16_t adr1 = cpu.reg[0] + com.off1;
		uint16_t adr2 = cpu.reg[1];
		break;
	}
	case 11 : 
	{
		uint16_t adr1 = cpu.reg[0] + com.off1;
		uint16_t adr2 = cpu.reg[1] + com.off2;
		break;
	}
	}

	datatype32 res;
	res.w32.i32 = calculate(cpu.memory[adr1].w32.i32, cpu.memory[adr2].w32.i32);
	cpu.memory.setMemory(adr1, res);
	change_flag(res.w32.i32, cpu);
}