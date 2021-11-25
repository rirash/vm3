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

bool loader(const std::string& filename, Processor& cpu)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;
    std::string str;
    char teg;
    bool end = false;
    uint16_t adr = 0;
    file >> str;

    if (str[0] != 'a')
    {
        std::cout << "\nerror a!\n";
        return false;
    }
    else
    {
        file >> str;
        adr = (uint16_t)std::stoi(str);
        std::getline(file, str);
        while (!file.eof() && !end)
        {
            file >> str;
            teg = str[0];
            switch (teg)
            {
            case 's':
            {
                file >> str;
                cpu.psw.setIP((uint16_t)std::atoi(str.c_str()));
                std::getline(file, str);
                end = true;
                break;
            }

            case 'c': //команда
            {
                file >> str;
                uint8_t kop = std::atoi(str.c_str());
                if (kop >= 27 && kop <= 31) // 2 byte
                {
                    datatype16 w16;
                    w16.cmd16.KOP = kop;
                    file >> str;
                    w16.cmd16.bb = std::stoi(str);
                    file >> str;
                    w16.cmd16.off = std::stoi(str);
                    cpu.memory.setMemory(adr, w16);
                    adr += 2;
                }
                else
                {
                    if ((kop >= 1 && kop <= 2) || (kop >= 23 && kop <= 26)) // 3 byte
                    {
                        datatype24 w24;
                        w24.cmd24.KOP = kop;
                        file >> str;
                        w24.cmd24.bb = std::stoi(str);
                        file >> str;
                        w24.cmd24.adr = std::stoi(str);
                        cpu.memory.setMemory(adr, w24);
                        adr += 3;
                    }
                    else // 4 byte
                    {
                        datatype32 w32;
                        w32.cmd32.KOP = kop;
                        file >> str;
                        w32.cmd32.bb = std::stoi(str);;
                        file >> str;
                        w32.cmd32.off1 = std::stoi(str);;
                        file >> str;
                        w32.cmd32.off2 = std::stoi(str);
                        adr += 4;
                    }
                }               
                std::getline(file, str);
                break;
            }

            case 'I': // знаковые числа в 32 бита
            {
                file >> str;
                datatype32 w32;
                w32.w32.i32 = std::stoi(str);
                cpu.memory.setMemory(adr, w32);
                adr += 4;
                std::getline(file, str);
                break;
            }

            case 'F':
            {
                file >> str;
                datatype32 w32;
                w32.w32.f = std::stof(str);
                cpu.memory.setMemory(adr, w32);                
                adr += 4;
                std::getline(file, str);
                break;
            }

            default:
            {
                std::cout << "\nerror teg!\n";
                return false;
            }
            }
        }
    }
    file.close();
    return true;
}
