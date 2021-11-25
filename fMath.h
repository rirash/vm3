#ifndef FMATH_H
#define FMATH_H
#include <cmath>
#include "Command.h"
#include <stdexcept>

class fMath : public Command
{
	void change_flag(float res, Processor& cpu) noexcept;
	virtual float calculate(float left, float right) = 0;
	void operator()(Processor& cpu);
};

class fAdd : public fMath
{
	float calculate(float left, float right) { return left + right; }
};

class fSub : public fMath
{
	float calculate(float left, float right) { return left - right; }
};

class fMul : public fMath
{
	float calculate(float left, float right) { return left * right; }
};

class fDiv : public fMath
{
	float calculate(float left, float right)
	{
		if (fabs(right) < eps) throw std::invalid_argument("Деление на ноль невозможно!");
		return left / right;
	}
};

class Sqrt : public fMath
{
	float calculate(float left, float right) { return (left >= 0) ? (float)sqrt(left) : 0; }
};
#endif//! FMATH_H