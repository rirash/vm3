#ifndef IN_OUT_H
#define IN_OUT_H
#include "Command.h"
#include <iostream>

class IO : public Command
{
	void operator()(Processor& cpu) noexcept override;
	virtual void process(uint16_t adr, Processor& cpu) = 0;
};

class InputI : public IO
{
	virtual void process(uint16_t adr, Processor& cpu);
};

class OutputI : public IO
{
	virtual void process(uint16_t adr, Processor& cpu);
};

class InputF : public IO
{
	virtual void process(uint16_t adr, Processor& cpu);
};

class OutputF : public IO
{
	virtual void process(uint16_t adr, Processor& cpu);
};
#endif//! IN_OUT_H