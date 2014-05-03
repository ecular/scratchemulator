#include <stdio.h>
#include <pthread.h>
#include <SDL/SDL.h>
#include <iostream>
#include "cpu.h"
#include "video.h"
#include "disk_handle.h"
#include "timer.h"
#include "display.h"

void print_screen(Cpu  *cpu)
{
    int x, y;
    // print out the screen
    for(y = 0; y < 25; ++y)
    {
        for(x = 0; x < 80; ++x)
            fprintf(stdout, "%c", cpu->ram[ 0x8000 + (y * 80) + x ]);
        fprintf(stdout, "\n");
    }
}


void *RunThread(void *cpu_arg)
{
    Cpu *cpu = (Cpu *)cpu_arg;
    //while(true)
    {
        cpu->Exec(100000000);
    }
}

//int main(int argc, char **argv)
//{
//    SDL_Surface *pScreen = 0;
//    try {
//        if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
//            throw "Could not initliaze SDL";
//    }
//    catch(const char *s)
//    {
//        std::cerr << s << std::endl;
//        return -1;
//    }
//
//    std::cout << "SDL initialized.\n";
//    pScreen = SDL_SetVideoMode(640, 400, 32, SDL_SWSURFACE);
//    SDL_Flip(pScreen);
//    SDL_Delay(2000);
//    Interrupt_Controller_8259a i8259a;
//    Interval_Timer_8253 i8253;
//    port_handle ports_operate();
//    Video video;
//    disk_handle Disk_handle;
//    FILE* binFile = NULL;
//    const char* inputFilename = argv[1];
//    size_t fileSize = 0;
//    Cpu cpu;
//    binFile = fopen(inputFilename, "rb");
//    if(binFile)
//    {
//        fseek(binFile, 0, SEEK_END);
//        fileSize = (size_t)ftell(binFile);
//        fseek(binFile, 0, SEEK_SET);
//        cpu.Init(0x10000);
//        cpu.seti8259a(&i8259a);
//        cpu.setDisk_handle(&Disk_handle);
//        fread(cpu.ram, 1, fileSize, binFile);
//    }
//    cpu.SetDS(0);
//    cpu.SetCS(0);
//    cpu.SetSS(0);
//    cpu.SetIP(0);
//    cpu.SetSP(0x100);
//    while(cpu.halt == 0)
//        cpu.Exec();
//    print_screen(&cpu);

/*
   Cpu cpu;

   uint16_t disp16 = 0x1234;
   uint8_t disp8 = 0x34;
   cpu.Init(0x10000);
   cpu.SetCS(0x10);
   cpu.SetIP(0x20);
   cpu.SetDS(0x200);
   cpu.SetAL(0xcc);
   cpu.SetCL(0x33);

   cpu.WriteRam8(0x10 * 16 + 0x20, 0xf6);
   cpu.WriteRam8(0x10 * 16 + 0x21, 0xe1);
//cpu.WriteRam8(0x10 * 16 + 0x22, 0x46);
//cpu.WriteRam8(0x10 * 16 + 0x23, 0x00);
//cpu.WriteRam8(0x10 * 16 + 0x24, 0x99);
//cpu.WriteRam8(0x10 * 16 + 0x25, 0x33);


cpu.Exec();

//cpu.WriteRam8(0x10 * 16 + 0x26, 0xc7);
//cpu.WriteRam8(0x10 * 16 + 0x27, 0x46);
//cpu.WriteRam8(0x10 * 16 + 0x28, 0x00);
//cpu.WriteRam8(0x10 * 16 + 0x29, 0x78);
//cpu.WriteRam8(0x10 * 16 + 0x2A, 0x56);

//    cpu.Exec();
printf("AX:%x\n",cpu.GetAX());
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
//cpu.SetES(0);
//cpu.WriteRam16(cpu.GetDS() * 16 + 0x1234, 0x7788);
//cpu.SetCL(0x1);
//cpu.WriteRam16(cpu.GetDS() * 16 + 0x1234, 0x3333);
//cpu.SetBX(0x1200);

//printf("ax:%x\n", cpu.GetAX());
//printf("bx:%x\n", cpu.GetBX());
//printf("cx:%x\n", cpu.GetCX());
//printf("dx:%x\n", cpu.GetDX());
//printf("si:%x\n", cpu.GetSI());
//printf("bp:%x\n", cpu.GetBP());
//printf("bx:%x\n", cpu.GetBX());
//printf("sp:%x\n", cpu.GetSP());
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
//printf("%x\n", cpu.GetDX());
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
    */
int main(int argc, char **argv)
{
    SDL_Surface *pScreen = 0;
    pthread_t runthread;
    try
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
            throw "Could not initliaze SDL";
    }
    catch(const char *s)
    {
        std::cerr << s << std::endl;
        return -1;
    }

    std::cout << "SDL initialized.\n";

    Cpu cpu;
    cpu.Init(0x100000);
    cpu.Reset();

    Interval_Timer_8253 i8253;
    std::cout << "8253 timer initialized." << std::endl;

    Interrupt_Controller_8259a i8259a;
    std::cout << "8259a Interrupt Controller initialized." << std::endl;

    Video video;
    video.setcpu(&cpu);
    std::cout << "video device initialized." << std::endl;

    Display display;
    display.setcpu(&cpu);
    display.setvideo(&video);

    timer timing;
    timing.seti8259a(&i8259a);
    timing.seti8253(&i8253);
    timing.setvideo(&video);

    Disk fb("./dos-boot.img", 0);
    fb.setcpu(&cpu);

    disk_handle Disk_handle;
    Disk_handle.setcpu(&cpu);
    Disk_handle.insert_disk(0, &fb);

    port_handle ports_operate;
    ports_operate.seti8259a(&i8259a);
    ports_operate.seti8253(&i8253);
    ports_operate.setvideo(&video);

    cpu.setports_operate(&ports_operate);
    cpu.seti8259a(&i8259a);
    cpu.seti8253(&i8253);
    cpu.setvideo(&video);
    cpu.setDisk_handle(&Disk_handle);
    cpu.settimer(&timing);

    timing.timer_init();
    display.InitDisplay();



    // FILE* binFile = NULL;
    // const char* inputFilename = argv[1];
    // size_t fileSize = 0;
    // binFile = fopen(inputFilename, "rb");
    // if(binFile)
    // {
    //     fseek(binFile, 0, SEEK_END);
    //     fileSize = (size_t)ftell(binFile);
    //     fseek(binFile, 0, SEEK_SET);
    //     cpu.Init(0x10000);
    //     fread(cpu.ram, 1, fileSize, binFile);
    // }

    if(cpu.LoadBIOS("./pcxtbios.bin"))
        std::cout << "BIOS load success..." << std::endl;
    if(cpu.LoadRom(0xF6000, "./rombasic.bin"))
        std::cout << "rombasic load success..." << std::endl;
    if(cpu.LoadRom(0xC0000, "./videorom.bin"))
        std::cout << "videorom load success..." << std::endl;

    pthread_create(&runthread, NULL, RunThread, (void *)(&cpu));
    // cpu.SetDS(0);
    // cpu.SetCS(0);
    // cpu.SetSS(0);
    // cpu.SetIP(0);
    // cpu.SetSP(0x100);
    // while(cpu.halt == 0)
    //     cpu.Exec(1);
    // print_screen(&cpu);
    pthread_join(runthread, NULL);
    return 0;
}
