#include <stdio.h>

#include "cpu.h"

int main()
{
    Cpu cpu;
    uint16_t disp16 = 0x1234;
    uint8_t disp8 = 0x12;
    cpu.Init(0x10000);
    cpu.SetCS(0x10);
    cpu.SetIP(0x20);
    //cpu.SetAX(0x1234);
    //cpu.SetBX(0x5);
    //cpu.SetCX(0x6789);
    //cpu.SetDX(0xabcd);


    cpu.WriteRam8(0x10 * 16 + 0x20, 0x05);
    //cpu.WriteRam8(0x10 * 16 + 0x21, 0x34);
    //cpu.WriteRam8(0x10*16+0x22,disp8);
    cpu.WriteRam16(0x10 * 16 + 0x21, disp16);
    cpu.SetSI(0x8);
    cpu.SetDS(0x2);
    cpu.SetDI(0x3);
    cpu.SetBP(0x4);
    //cpu.WriteRam16(cpu.GetDS() * 16 + cpu.GetBX() +cpu.GetSI() + disp16, 0x5678);
    cpu.Exec();
    printf("%x\n", cpu.GetAX());

    //    cpu.Init(0x100);
    //    cpu.WriteRam8(20, value1);
    //    cpu.WriteRam8(21, value2);
    //    printf("20:%x 21:%x\n", cpu.ReadRam8(20), cpu.ReadRam8(21));
    //    printf("20(16bit)%x\n", cpu.ReadRam16(20));
    //    uint16_t value3 = 0x5678;
    //    void  *p = (void *)(&value3);
    //    printf("%x\n", *((uint8_t *)p));
    //cpu.SetddddDH(value1);
    //cpu.SetDL(value2);
    //printf("DH:%x\n", cpu.GetDH());
    //printf("DL:%x\n", cpu.GetDL());
    //printf("DX:%x\n", cpu.GetDX());

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
