#include "in_out.h"

void IO::operator()(Processor& cpu) noexcept
{
	process(cpu.get_Command24().adr, cpu);
}

void InputI::process(uint16_t adr, Processor& cpu)
{
	datatype32 val;
	std::cin >> val.w32.i32;
	cpu.memory.setMemory(adr, val);
}

void OutputI::process(uint16_t adr, Processor& cpu)
{
	std::cout << cpu.memory[adr].w32.i32 << "\n";
}

void InputF::process(uint16_t adr, Processor& cpu)
{
	datatype32 val;
	std::cin >> val.w32.f;
	cpu.memory.setMemory(adr, val);
}

void OutputF::process(uint16_t adr, Processor& cpu)
{
	std::cout << cpu.memory[adr].w32.f << "\n";
}