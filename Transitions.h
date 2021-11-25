#ifndef TRANSITIONS_H
#define TRANSITIONS_H
#include "Command.h"

class Jmp : public Command
{
public:
	void go_to(Processor& cpu)
	{
		cmd16 com = cpu.get_Command16();
		uint16_t adr = 0;
		switch (com.bb)
		{
		case 00: adr = cpu.psw.getIP() + com.off; break;
		case 10: adr = cpu.reg[0] + com.off; break;
		case 01: adr = cpu.reg[1] + com.off; break;
		}
		cpu.psw.setIP(adr);
	}

	void operator()(Processor& cpu) noexcept override { go_to(cpu); }
};

class JmpZF : public Jmp
{
	void operator()(Processor& cpu) noexcept override
	{
		if (cpu.psw.getZF() == 1) go_to(cpu);
		else cpu.psw.setIP(cpu.psw.getIP() + 2);
	}
};

class JmpNZF : public Jmp
{
	void operator()(Processor& cpu) noexcept override
	{
		if (cpu.psw.getZF() == 0) go_to(cpu);
		else cpu.psw.setIP(cpu.psw.getIP() + 2);
	}
};

class JmpSF : public Jmp
{
	void operator()(Processor& cpu) noexcept override
	{
		if (cpu.psw.getSF() == 1) go_to(cpu);
		else cpu.psw.setIP(cpu.psw.getIP() + 2);
	}
};

class JmpNSF : public Jmp
{
	void operator()(Processor& cpu) noexcept override
	{
		if (cpu.psw.getSF() == 0) go_to(cpu);
		else cpu.psw.setIP(cpu.psw.getIP() + 2);
	}
};

#endif //!TRANSITIONS_H