#ifndef PSW_H
#define PSW_H
#include <cstdint>

class PSW
{
private:
    uint16_t IP : 16;                                   //IP-адрес
    uint16_t ZF : 1;                                    //Флаг нуля
    uint16_t SF : 1;                                    //Флаг знака
    uint16_t reserved : 14;                             //Резерв
public:
    PSW() : IP(0), ZF(0), SF(0) {};
    uint16_t getIP() const noexcept { return IP; }      //Получить значение IP
    uint8_t getZF() const noexcept { return ZF; }       //Получить значение ZF
    uint8_t getSF() const noexcept { return SF; }       //Получить значение SF

    void setIP(uint16_t adr) noexcept { IP = adr; }     //Положить значение IP
    void setZF(bool val) noexcept                    //Положить значение ZF
    {
        if (val) ZF = 1;
        else ZF = 0;
    }
    void setSF(bool val) noexcept                    //Положить значение SF
    {
        if (val) SF = 1;
        else SF = 0;
    }
};
#endif // PSW_H
