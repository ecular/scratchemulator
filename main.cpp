#include <stdio.h>

#include "cpu.h"

int main()
{
    Cpu cpu;
    uint16_t value1 = 0x1234;
    uint16_t value2 = 0x5678;

    cpu.SetSP(value1);
    cpu.SetBP(value2);
    printf("%x\n", cpu.GetSP());
    printf("%x\n", cpu.GetBP());

    cpu.SetSI(value1);
    cpu.SetDI(value2);
    printf("%x\n", cpu.GetSI());
    printf("%x\n", cpu.GetDI());

    cpu.SetIP(value1);
    cpu.SetFlag(value2);
    printf("%x\n", cpu.GetIP());
    printf("%x\n", cpu.GetFlag());

    cpu.SetCS(value1);
    cpu.SetDS(value2);
    printf("%x\n", cpu.GetCS());
    printf("%x\n", cpu.GetDS());


    cpu.SetSS(value1);
    cpu.SetES(value2);
    printf("%x\n", cpu.GetSS());
    printf("%x\n", cpu.GetES());
    return 0;
}
