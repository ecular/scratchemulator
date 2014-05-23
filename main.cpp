#include <stdio.h>
#include <pthread.h>
#include <SDL/SDL.h>
#include <iostream>
#include "cpu.h"
#include "video.h"
#include "disk_handle.h"
#include "timer.h"
#include "display.h"
#include "keyboard.h"

struct InputArg {
    Cpu *cpu_arg;
    Keyboard *keyboard_arg;
};

void *RunThread(void *cpu_arg)
{
    Cpu *cpu = (Cpu *)cpu_arg;
    while(!cpu->GetStatus())
    {
        cpu->Exec(1000000);
    }
    return NULL;
}

void *InputThread(void *keyboard_arg)
{
    struct InputArg *arg = (struct InputArg *)keyboard_arg;
    arg->keyboard_arg->MonitorInput();
    arg->cpu_arg->SetStatus(true);
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t runthread;
    pthread_t inputthread;
    struct InputArg arg;
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
    std::cout << "Display device initialized." << std::endl;

    Keyboard keyboard;
    keyboard.InitKeyboard(&i8259a);
    std::cout << "Keyboard device initialized." << std::endl;

    timer timing;
    timing.seti8259a(&i8259a);
    timing.seti8253(&i8253);
    timing.setvideo(&video);
    std::cout << "System timer initialized." << std::endl;

    Disk fb("./dos-boot.img", 0);
    fb.setcpu(&cpu);

    disk_handle Disk_handle;
    Disk_handle.setcpu(&cpu);
    Disk_handle.insert_disk(0, &fb);
    std::cout << "Disk controller initialized." << std::endl;

    port_handle ports_operate;
    ports_operate.seti8259a(&i8259a);
    ports_operate.seti8253(&i8253);
    ports_operate.setvideo(&video);
    ports_operate.setkeyboard(&keyboard);

    cpu.setports_operate(&ports_operate);
    cpu.seti8259a(&i8259a);
    cpu.seti8253(&i8253);
    cpu.setvideo(&video);
    cpu.setDisk_handle(&Disk_handle);
    cpu.settimer(&timing);

    timing.timer_init();
    display.InitDisplay();

    if(cpu.LoadBIOS("./bios.bin"))
        std::cout << "load BIOS Success..." << std::endl;
    if(cpu.LoadRom(0xF6000, "./ibm-basic-1.00.rom"))
        std::cout << "load Basicrom Success..." << std::endl;
    if(cpu.LoadRom(0xC0000, "./videorom.bin"))
        std::cout << "load Videorom Success..." << std::endl;

    /*run CPU*/
    pthread_create(&runthread, NULL, RunThread, (void *)(&cpu));
    /*start SDL event listen*/
    arg.cpu_arg = &cpu;
    arg.keyboard_arg = &keyboard;
    pthread_create(&inputthread, NULL, InputThread, (void *)(&arg));

    pthread_join(runthread, NULL);
    return 0;
}
