#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>

class Memory
{
    const int max_size = 65536;
private:
    uint8_t* memory;                                        //Наша память
public:
    Memory() { memory = new uint8_t[max_size]; }
    Memory(Memory&) = delete;
    Memory operator = (Memory&) = delete;

    ~Memory() { delete[] memory; }

    datatype32 operator[](uint16_t adr) noexcept //Получение 32-битного числа
    { 
        datatype32 w32;
        w32.byte[0] = memory[adr];
        w32.byte[1] = memory[adr + 1];
        w32.byte[2] = memory[adr + 2];
        w32.byte[3] = memory[adr + 3];
        return w32; 
    }

    datatype8 getByte(uint16_t adr) noexcept //Получение 32-битного числа
    {
        datatype8 w8;
        w8.byte = memory[adr];
        return w8;
    }

    void setMemory(uint16_t adr, datatype32 w32) noexcept
    {
        memory[adr] = w32.byte[0];
        memory[adr + 1] = w32.byte[1];
        memory[adr + 2] = w32.byte[2];
        memory[adr + 3] = w32.byte[3];
    }

    void setMemory(uint16_t adr, datatype16 w16) noexcept 
    {
        memory[adr] = w16.byte[0];
        memory[adr + 1] = w16.byte[1];
    }

    void setMemory(uint16_t adr, datatype24 w24) noexcept
    {
        memory[adr] = w24.byte[0];
        memory[adr + 1] = w24.byte[1];
        memory[adr + 2] = w24.byte[2];
    }
};
#endif // MEMORY_H
