#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <stdexcept>
#include <iomanip>


	bool isDigit(std::string str)													//проверка подстроки на число (и целое, и вещественное)
	{
		std::size_t IdxOfPoint = str.find(',', 0);

		if (IdxOfPoint != -1)
		{
			std::string temp[2];
			temp[0] = str.substr(0, IdxOfPoint);
			temp[1] = str.substr(IdxOfPoint + 1);

			for (int i = 0; i < 2; i++)
			{
				if (!std::all_of(temp[i].begin(), temp[i].end(), ::isdigit)) return false;
			}
		}
		else
		{
			if (!std::all_of(str.begin(), str.end(), ::isdigit)) return false;
		}

		return true;
	}

	bool trueElement(char ch)														//проверка на отсутствие лишних символом
	{
		if (std::isdigit(ch)) return true;
		else
		{
			std::string temp = " +-*/()^sqrt";
			if (temp.find(ch) != std::string::npos) return true;
		}
		return false;
	}

	int getPriority(char ch)														//приоритет операций
	{
		switch (ch)
		{
		case '(': return 0;
		case ')': return 1;
		case '+': return 2;
		case '-': return 3;
		case '*': return 4;
		case '/': return 4;
		case '^': return 5;
		case 's': return 5;	  //корень, за операция считаем только первую букву, остальные буквы - пропускаем
		default: return 6;
		}
	}

	bool isOper(char ch)														//проверка на операцию
	{
		std::string temp = "+-*/^()s";
		std::size_t c = temp.find(ch);
		return c != -1;
	}

	void ifIsOperation(std::string& outputString, std::stack<char>& operationStack, char ch)	//перевод операции в обратную польскую запись 
	{
		if (operationStack.size() > 0)
		{
			if (getPriority(ch) <= getPriority(operationStack.top()))
			{
				outputString += operationStack.top();
				outputString += " ";
				operationStack.pop();
			}
		}

		operationStack.push(ch);
	}

	void ifIsBracket(std::string& outputString, std::stack<char>& operationStack, char ch)		//перевод скобок в опз
	{
		if (ch == '(') operationStack.push(ch);
		else
		{
			char temp = operationStack.top();
			operationStack.pop();

			while (temp != '(')
			{
				outputString += temp;
				temp = operationStack.top();
				operationStack.pop();
				outputString += " ";
			}
		}
	}

	long double startOperation(long double first, long double second, char op)
	{
		switch (op)
		{
		case '+': return first + second;
		case '-': return first - second;
		case '*': return first * second;
		case '/': return first / second;
		case 's':
		{
			if (first >= 0.0000000000000000) return sqrt(first);
			else throw std::invalid_argument("Некорректное выражение под знаком корня!");
		}
		case '^': return pow(first, second);
		default: return 0;
		}

	}

	bool isDelim(char ch)															//если символ - разделитель(пробел или продолжение корня)
	{
		std::string temp = " qrt";
		std::size_t c = temp.find(ch);
		return c != -1;
	}

	std::string RPE(std::string inputString)
	{
		std::string outputString = "";
		std::stack<char> operationStack;
		bool operation = false, digit = false;


		for (int i = 0; i < inputString.length(); i++)
		{
			if (trueElement(inputString[i]))
			{
				if (operation && isOper(inputString[i]) && inputString[i] != ')' && inputString[i] != '(') throw std::invalid_argument("Некорректная строка!");
				if (digit && std::isdigit(inputString[i])) throw std::invalid_argument("Некорректная строка!");

				if (!isDelim(inputString[i]))
				{
					digit = true;
					operation = false;
					if (std::isdigit(inputString[i]))
					{
						std::string thisDigit = "";
						while ((std::isdigit(inputString[i]) || inputString[i] == ',') && i < inputString.length())
						{
							thisDigit += inputString[i];
							i++;
						}
						i--;
						if (isDigit(thisDigit)) outputString += thisDigit + ' ';
					}

					if (isOper(inputString[i]))
					{
						digit = false;
						operation = (inputString[i] != ')' && inputString[i] != '(');

						if (inputString[i] == ')' || inputString[i] == '(')
						{
							ifIsBracket(outputString, operationStack, inputString[i]);
						}
						else
						{
							ifIsOperation(outputString, operationStack, inputString[i]);
						}
					}
				}
			}
			else throw std::invalid_argument("Некорректная строка!");
		}

		while (operationStack.size() > 0)
		{
			if (operationStack.top() == '(' || operationStack.top() == ')') throw std::invalid_argument("Некорректная строка!");
			outputString += operationStack.top();
			operationStack.pop();
		}

		return outputString;
	}

	double counting(std::string RPE)
	{
		std::stack<double> digitStack;

		for (int i = 0; i < RPE.length(); i++)
		{
			if (std::isdigit(RPE[i]))
			{
				std::string temp = "";
				while (RPE[i] != ' ' && !isOper(RPE[i]))
				{
					temp += RPE[i];
					i++;
					if (i == RPE.length()) break;
				}
				digitStack.push(std::stod(temp));
				i--;
			}

			if (isOper(RPE[i]))
			{
				if (RPE[i] == 's')
				{
					long double temp = digitStack.top();
					digitStack.pop();
					digitStack.push(startOperation(temp, 0.0, RPE[i]));			//второй операнд при вычислении корня обозначим нулем, он не нужен
				}
				else
				{
					long double first = digitStack.top();
					digitStack.pop();
					long double second = digitStack.top();
					digitStack.pop();

					digitStack.push(startOperation(second, first, RPE[i]));
				}
			}
		}

		return digitStack.top();
	}

	std::string calcReversePolishEntry(std::string inputString)									    //преобразуем выражение в ОПЗ, вычисляем итоговый результат
	{
		std::string RPEString = RPE(inputString);
		double res = counting(RPEString);
		std::string s = std::to_string(res);

		if (s[s.size() - 1] == '0')
			for (size_t i = s.size() - 1; s[i] == '0'; i--)
				s.erase(i, 1);

		if (s[s.size() - 1] == '.')
			s.erase(s.size() - 1, 1);

		return s;
	}
