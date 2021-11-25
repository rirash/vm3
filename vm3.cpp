#include <iostream>
#include <fstream>
#include "Loader.h"
#include "ParseAssembler.h"


int main()
{
    translatorToMachineCode();
    setlocale(0, "rus");

    Processor cpu;
    if (loader("MachineCode.txt", cpu))
    {
        std::cout << "Загрузка прошла успешно.\n";
        cpu.run(true);
        std::cout << "\nПрограмма успешно выполнена.\n";
    }
    else std::cout << "Не удалось загрузить файл.\n";
    system("pause");

    return 0;
}
