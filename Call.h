#ifndef CALL_H
#define CALL_H
#include "Command.h"

class Call : public Command
{
public:
	void go_to(Processor& cpu)
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

		cpu.psw.setIP(adr1);
		cpu.reg[1] = adr2;
	}

	void operator()(Processor& cpu) noexcept override { go_to(cpu);}
};

class CallZF : public Call
{
	void operator()(Processor& cpu) noexcept override
	{
		if (cpu.psw.getZF() == 1) go_to(cpu);
		else cpu.psw.setIP(cpu.psw.getIP() + 4);
	}
};

class CallNZF : public Call
{
	void operator()(Processor& cpu) noexcept override
	{
		if (cpu.psw.getZF() == 0) go_to(cpu);
		else cpu.psw.setIP(cpu.psw.getIP() + 4);
	}
};

class CallSF : public Call
{
	void operator()(Processor& cpu) noexcept override
	{
		if (cpu.psw.getSF() == 1) go_to(cpu);
		else cpu.psw.setIP(cpu.psw.getIP() + 4);
	}
};

class CallNSF : public Call
{
	void operator()(Processor& cpu) noexcept override
	{
		if (cpu.psw.getSF() == 0) go_to(cpu);
		else cpu.psw.setIP(cpu.psw.getIP() + 4);
	}
};

class Return : public Call
{
	void operator()(Processor& cpu) noexcept
	{
		cpu.psw.setIP(cpu.reg[1]);
	}
};

#endif //!CALL_H