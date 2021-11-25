#ifndef COMMAND_H
#define COMMAND_H
#include <numeric>
#include "Types.h"
#include "Memory.h"
#include "PSW.h"

const double eps = 0.00000000000001;
class Processor;
class Command
{
public:
	virtual void operator() (Processor& cpu) = 0;
};

class Processor
{
private:
	allCommand cmd;
	Command* command[40];
	bool JF;
public:
	PSW psw;
	Memory memory;
	uint16_t reg[2];

	Processor();

	Processor(Processor&) = delete;
	Processor(Processor&&) = delete;
	Processor operator= (Processor&) = delete;
	Processor operator= (Processor&&) = delete;

	inline cmd16 get_Command16() const noexcept { return cmd.dt32.dt16[0].cmd16; }
	inline cmd24 get_Command24() const noexcept { return cmd.dt24.cmd24; }
	inline cmd32 get_Command32() const noexcept { return cmd.dt32.cmd32; }
	inline allCommand get_Command() const noexcept { return cmd; }

	void debug() const noexcept;
	void run(bool debug_ver);
};

#endif //!COMMAND_H