#include <iostream>
#include <string>
#include <fstream>
#include "Command.h"
#include "Compare.h"
#include "fMath.h"
#include "iMath.h"
#include "in_out.h"
#include "Transitions.h"
#include "Move.h"
#include "Stop.h"
#include "Call.h"

enum Operations : uint8_t
{
	//Командра стоп
	stop = 0,

	//Загрузка в регистры
	move_r1 = 1,
	move_r2 = 2,

	//Целочисленная арифметика
	i_add = 3,
	i_sub = 4,
	i_mul = 5,
	i_div = 6,
	rem = 7,
	inc = 8,
	dec = 9,
	neg = 10,

	//Битовые операции
	b_and = 11,
	b_or = 12,
	b_xor = 13,
	b_not = 14,
	sdv_l = 15,
	sdv_r = 16,

	//Вещественная арифметика
	f_add = 17,
	f_sub = 18,
	f_mul = 19,
	f_div = 20,
	f_sqrt = 21,

	//Сравнения
	cmp = 22,

	//Ввод-вывод
	i_in = 23,
	f_in = 24,
	i_out = 25,
	f_out = 26,

	//Переходы
	jmp = 27,
	jzf = 28,
	jnzf = 29,
	jsf = 30,
	jnsf = 31,

	//Вызов к подпрограмме
	p_jmp = 32,
	p_jzf = 33,
	p_jnzf = 34,
	p_jsf = 35,
	p_jnsf = 36,

	ret = 37,
};

Processor::Processor()
{
	command[stop] = new Stop();

	command[move_r1] = new MoveR1();
	command[move_r2] = new MoveR2();

	command[i_add] = new iAdd();
	command[i_sub] = new iSub();
	command[i_mul] = new iMul();
	command[i_div] = new iDiv();
	command[rem] = new Remainder();
	command[inc] = new Inc();
	command[dec] = new Dec();
	command[neg] = new Neg();

	command[b_and] = new And();
	command[b_or] = new Or();
	command[b_xor] = new Xor();
	command[b_not] = new Not();
	command[sdv_l] = new Sdvig_left();
	command[sdv_r] = new Sdvig_right();

	command[f_add] = new fAdd();
	command[f_sub] = new fSub();
	command[f_mul] = new fMul();
	command[f_div] = new fDiv();
	command[f_sqrt] = new Sqrt();

	command[cmp] = new Cmp();

	command[i_in] = new InputI();
	command[f_in] = new InputF();
	command[i_out] = new OutputI();
	command[f_out] = new OutputF();

	command[jmp] = new Jmp();
	command[jzf] = new JmpZF();
	command[jnzf] = new JmpNZF();
	command[jsf] = new JmpSF();
	command[jnsf] = new JmpNSF();

	command[p_jmp] = new Call();
	command[p_jzf] = new CallZF();
	command[p_jnzf] = new CallNZF();
	command[p_jsf] = new CallSF();
	command[p_jnsf] = new CallNSF();

	command[ret] = new Return();

	JF = false;

	psw.setIP(0);
	psw.setSF(0);
	psw.setZF(0);

	cmd.dt24.byte[0] = 0;
	cmd.dt24.byte[1] = 0;
	cmd.dt24.byte[2] = 0;
	cmd.dt32.byte[0] = 0;
	cmd.dt32.byte[1] = 0;
	cmd.dt32.byte[2] = 0;
	cmd.dt32.byte[3] = 0;

	reg[0] = 0;
	reg[1] = 0;
}

void Processor::debug() const noexcept
{
	allCommand cmd = get_Command();
	if (cmd.dt32.dt16->cmd16.KOP >= 27 && cmd.dt32.dt16->cmd16.KOP <= 31)
	{
		std::cout << "\nВыполняемая команда: КОП:" << cmd.dt32.dt16->cmd16.KOP << " bb:" <<
			cmd.dt32.dt16->cmd16.bb << " off:" << cmd.dt32.dt16->cmd16.off << " flags:" << (int)psw.getZF() << " "
			<< (int)psw.getSF() << " IP:" << psw.getIP() << "\n";
	}
	else
	{
		if ((cmd.dt24.cmd24.KOP >= 1 && cmd.dt24.cmd24.KOP <= 2) || (cmd.dt24.cmd24.KOP >= 23 && cmd.dt24.cmd24.KOP <= 26))
		{
			std::cout << "\nВыполняемая команда: КОП:" << cmd.dt24.cmd24.KOP << " bb:" <<
				cmd.dt24.cmd24.bb << " adr:" << cmd.dt24.cmd24.adr << " flags:" << (int)psw.getZF() << " "
				<< (int)psw.getSF() << " IP:" << psw.getIP() << "\n";
		}
		else
		{
			std::cout << "\nВыполняемая команда: КОП:" << cmd.dt32.cmd32.KOP << " bb:" <<
				cmd.dt32.cmd32.bb << " off1:" << cmd.dt32.cmd32.off1 << " off2:" << cmd.dt32.cmd32.off2
				<< " flags:" << (int)psw.getZF() << " " << (int)psw.getSF() << " IP:" << psw.getIP() << "\n";
		}
	}
}

void Processor::run(bool debug_ver)
{
	datatype8 w8;
	do
	{
		w8 = memory.getByte(psw.getIP());
		if (w8.kb.KOP >= jmp && w8.kb.KOP <= jnsf)
		{
			cmd.dt32.dt16[0].byte[0] = memory.getByte(psw.getIP()).byte;
			cmd.dt32.dt16[0].byte[1] = memory.getByte(psw.getIP() + 1).byte;
			if (debug_ver) debug();
			command[cmd.dt32.dt16[0].cmd16.KOP]->operator()(*this);
			JF = true;
			psw.setIP(psw.getIP() + 2);
		}
		else
		{
			if ((w8.kb.KOP >= 1 && w8.kb.KOP <= 2) || (w8.kb.KOP >= i_in && w8.kb.KOP <= f_out))
			{
				cmd.dt24.byte[0] = memory.getByte(psw.getIP()).byte;
				cmd.dt24.byte[1] = memory.getByte(psw.getIP() + 1).byte;
				cmd.dt24.byte[2] = memory.getByte(psw.getIP() + 2).byte;
				if (debug_ver) debug();
				command[cmd.dt24.cmd24.KOP]->operator()(*this);
				JF = false;
				psw.setIP(psw.getIP() + 3);
			}
			else
			{
				cmd.dt32.byte[0] = memory.getByte(psw.getIP()).byte;
				cmd.dt32.byte[1] = memory.getByte(psw.getIP() + 1).byte;
				cmd.dt32.byte[2] = memory.getByte(psw.getIP() + 2).byte;
				cmd.dt32.byte[3] = memory.getByte(psw.getIP() + 3).byte;
				if (debug_ver) debug();
				command[cmd.dt32.cmd32.KOP]->operator()(*this);
				JF = false;
				psw.setIP(psw.getIP() + 4);
			}
		}
	} while (w8.kb.KOP != stop);
}