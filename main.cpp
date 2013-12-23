#include <stdio.h>

#include "cpu.h"

int main()
{
    Cpu cpu;
    uint8_t value1 = 0x12;
    uint8_t value2 = 0x34;

//    cpu.Init(0x100);
//    cpu.WriteRam8(20, value1);
//    cpu.WriteRam8(21, value2);
//    printf("20:%x 21:%x\n", cpu.ReadRam8(20), cpu.ReadRam8(21));
//    printf("20(16bit)%x\n", cpu.ReadRam16(20));
//    uint16_t value3 = 0x5678;
//    void  *p = (void *)(&value3);
//    printf("%x\n", *((uint8_t *)p));
    cpu.SetAH(value1);
    cpu.SetAL(value2);
    printf("AH:%x\n", cpu.GetAH());
    printf("AL:%x\n", cpu.GetAL());
    printf("AX:%x\n", cpu.GetAX());

    // cpu.SetIP(value1);
    // cpu.SetFlag(value2);
    // printf("%x\n", cpu.GetIP());
    // printf("%x\n", cpu.GetFlag());

    // cpu.SetCS(value1);
    // cpu.SetDS(value2);
    // printf("%x\n", cpu.GetCS());
    // printf("%x\n", cpu.GetDS());


    // cpu.SetSS(value1);
    // cpu.SetES(value2);
    // printf("%x\n", cpu.GetSS());
    // printf("%x\n", cpu.GetES());
    return 0;
}
