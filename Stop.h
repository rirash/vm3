#ifndef STOP_H
#define STOP_H
#include <iostream>
#include "Command.h"

class Stop : public Command
{
	void operator()(Processor& cpu) { std::cout << "\nПрограмма завершена."; }
};
#endif //!STOP_h
