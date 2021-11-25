#ifndef COMPARE_H
#define COMPARE_H
#include "Command.h"

class Cmp : public Command
{
public:
	void change_flag(float res, Processor& cpu);
	void operator()(Processor& cpu);
};

#endif //!IMATH_H
