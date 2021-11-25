#ifndef MOVE_H
#define MOVE_H
#include "Command.h"

class Move : public Command
{
	void operator()(Processor& cpu);
	virtual void process(cmd24 cmd, Processor& cpu) = 0;
};

class MoveR1 : public Move
{
	void process(cmd24 cmd, Processor& cpu);
};

class MoveR2 : public Move
{
	void process(cmd24 cmd, Processor& cpu);
};

#endif//! MOVE_H
