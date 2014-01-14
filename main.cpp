#include <stdio.h>

#include "cpu.h"

int main()
{
    Cpu cpu;
    uint16_t disp16 = 0x1234;
    uint8_t disp8 = 0x34;
    cpu.Init(0x10000);
    cpu.SetCS(0x10);
    cpu.SetIP(0x20);
    cpu.SetBP(0x21);
    cpu.SetSI(0x777);
    cpu.SetAX(0x00);
    //cpu.SetBX(0x5);
    cpu.SetCX(0x7659);
    //cpu.SetDX(0xabcd);
    cpu.SetFlag(0xFFFF);


    cpu.SetAX(0x00);
    cpu.SetBX(0x20);
    cpu.SetCX(0x0003);
    cpu.SetDX(0x1234);

    cpu.SetSP(0x1);
    cpu.SetBP(0x1);
    cpu.SetSI(0x1234);
    cpu.SetDI(0x1234);
    cpu.SetSS(0x12);
    cpu.SetDS(0x10);

    cpu.WriteRam8(0x10 * 16 + 0x20, 0xd4);
    cpu.WriteRam8(0x10 * 16 + 0x21, 0x03);
    cpu.WriteRam8(0x10 * 16 + 0x22, 0x03);
    //cpu.WriteRam16(0x10 * 16 + 0x22, disp16);
    //cpu.WriteRam16(0x10 * 16 + 0x24, 0x4444);
    //printf("main read:%x\n",cpu.ReadRam16(0x10 * 16 + 0x22));
//cpu.WriteRam8(0x10 * 16 + 0x22, 0x12);
    //cpu.WriteRam8(0x10*16+0x21,disp8);
    //cpu.WriteRam16(0x10 * 16 + 0x21, disp16);
    //cpu.SetSI(0x8);
    //cpu.SetDS(0x2);
    //cpu.SetDI(0x3);
    //cpu.SetBP(0x4);
    //cpu.SetES(0x1234);
    //cpu.WriteRam16(cpu.GetDS() * 16 + cpu.GetBX() +cpu.GetSI() + disp16, 0x5678);
    //cpu.WriteRam16(cpu.GetSS() * 16 + cpu.GetSP(), 0x1234);
    //cpu.Exec();
    //cpu.SetES(0);
    //cpu.WriteRam16(cpu.GetDS() * 16 + 0x1234, 0x7788);
    //cpu.SetCL(0x1);
    cpu.SetFlag(0x0);
    //cpu.WriteRam16(cpu.GetDS() * 16 + 0x1234, 0x3333);
    //cpu.SetBX(0x1200);
    cpu.SetAX(0x1234);

    //printf("ax:%x\n", cpu.GetAX());
    //printf("bx:%x\n", cpu.GetBX());
    //printf("cx:%x\n", cpu.GetCX());
    //printf("dx:%x\n", cpu.GetDX());
    //printf("si:%x\n", cpu.GetSI());
    //printf("bp:%x\n", cpu.GetBP());
    //printf("bx:%x\n", cpu.GetBX());
    //printf("sp:%x\n", cpu.GetSP());
    cpu.Exec();
    //printf("[sp]=%x\n", cpu.ReadRam16(cpu.GetSS() * 16 + cpu.GetSP()));
    //cpu.SetAX(0x12de);
    //cpu.SetBX(0x12de);
    //cpu.SetCX(0x12de);
    //cpu.SetDX(0x12de);

    //cpu.SetBP(0x12de);
    //cpu.SetDI(0x12de);

    //cpu.WriteRam8(0x10 * 16 + 0x23, 0x58);

    //cpu.Exec();
    //printf("ax:%x\n", cpu.GetAX());
    //printf("bx:%x\n", cpu.GetBX());
    //printf("cx:%x\n", cpu.GetCX());
    //printf("dx:%x\n", cpu.GetDX());
    //printf("si:%x\n", cpu.GetSI());
    //printf("bp:%x\n", cpu.GetBP());
    //printf("bx:%x\n", cpu.GetBX());
    //printf("sp:%x\n", cpu.GetSP());
    //printf("sP:%x\n", cpu.GetSP());
    //aprintf("%x\n", cpu.ReadRam16(cpu.GetDS() * 16 + cpu.GetBX()+cpu.GetSI()));
//printf("%x\n",cpu.ReadRam16(cpu.GetDS() * 16+0x1234));
    //printf("%x\n", cpu.GetAL());
    //printf("%x\n", cpu.ReadRam16(cpu.GetDS() * 16 + disp16));
    //printf("%x\n", cpu.ReadRam16(cpu.GetDS() * 16 + disp16));
    printf("%x\n", cpu.GetAX());
    //printf("%x\n", cpu.GetDX());
    printf("%x\n", cpu.GetFlag());
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
