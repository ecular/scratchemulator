#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <SDL/SDL.h>
#include <iostream>
#include "cpu.h"
#include "video.h"
#include "disk_handle.h"
#include "timer.h"
#include "display.h"
#include "keyboard.h"

using namespace std;

struct InputArg {
    Cpu *cpu_arg;
    Keyboard *keyboard_arg;
};

struct ConsoleArg {
    Cpu *cpu_arg;
    Disk **fd;
    disk_handle *Disk_handle;
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

void *ConsoleThread(void *console_arg)
{
    struct ConsoleArg *arg = (struct ConsoleArg *)console_arg;
    std::string input;
    std::cout << "\n\n\n\n" << std::endl ;
    while(true)
    {
        std::cout << "Console > ";
        getline(cin, input);
        if(input.substr(0, input.find(" ")) == "changefloppy")
        {
            arg->Disk_handle->eject_disk(0, *(arg->fd));
            delete *(arg->fd);
            *(arg->fd) = new Disk(input.substr(input.find(" ") + 1), 0);
            (*(arg->fd))->setcpu(arg->cpu_arg);
            arg->Disk_handle->insert_disk(0, *(arg->fd));
            std::cout << "OK" << std::endl;
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    std::cout << "ScratchEMU v0.0.1 (c) 2013-2014 Ecular " << std::endl;
    const char *short_opts = "cb:f:h:B:";
    const struct option long_opts[] =
    {
        {"command", no_argument, NULL, 'c'},
        {"bios", required_argument, NULL, 'b'},
        {"floppy", required_argument, NULL, 'f'},
        {"harddisk", required_argument, NULL, 'h'},
        {"boot", required_argument, NULL, 'B'},
        {0, 0, 0, 0}
    };
    opterr = 0;
    char c;

    pthread_t runthread;
    pthread_t inputthread;
    pthread_t consolethread;
    struct InputArg input_arg;
    struct ConsoleArg console_arg;
    /*caculate time*/
    // struct timeval tv;
    // uint64_t start_time;
    // uint64_t end_time;
    /**/
    Disk *fd1 = NULL;
    Disk *hd1 = NULL;

    char *floppyname = NULL;
    char *biosname = NULL;
    char *harddiskname = NULL;
    int boot = 0;
    int commandflag = 0;

    while((c = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
        switch(c)
        {
        case 'c' :
            commandflag = 1;
            break;
        case 'f' :
            floppyname = optarg;
            break;
        case 'b' :
            biosname = optarg;
            break;
        case 'h' :
            harddiskname = optarg;
            break;
        case 'B' :
            if(*optarg == 'f')
                boot = 0;
            if(*optarg == 'h')
                boot = 0x80;
            break;
        case '?' :
            std::cout << "Usage: semu -b <bios_img_name> [-f <floppy_img_name>] | [-h <harddisk_img_name>] | [-B <boot_device_id>] | [-c]" << std::endl;
            return 1;
        default:
            abort();
        }
    }

    if(biosname == NULL)
    {
        std::cout << "Require specify Bios file!" << std::endl;
        std::cout << "Usage: semu -b <bios_img_name> [-f <floppy_img_name>] | [-h <harddisk_img_name>] | [-B <boot_device_id>] | [-c]" << std::endl;
        return -1;
    }

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
    std::cout << "SDL initialized." << std::endl;

    Cpu cpu;
    cpu.Init(0x100000);
    cpu.Reset();
    cpu.SetBootDevice(boot);
    std::cout << "CPU initialized." << std::endl;

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

    disk_handle Disk_handle;
    Disk_handle.setcpu(&cpu);
    if(floppyname != NULL)
    {
        fd1 = new Disk(floppyname, 0);
        fd1->setcpu(&cpu);
        Disk_handle.insert_disk(0, fd1);
    }

    if(harddiskname != NULL)
    {
        hd1 = new Disk(harddiskname, 0x80);
        hd1->setcpu(&cpu);
        Disk_handle.insert_disk(0x80, hd1);
    }
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

    if(cpu.LoadBIOS(biosname))
        std::cout << "load BIOS Success..." << std::endl;
    if(cpu.LoadRom(0xF6000, "./ibm-basic-1.10.rom"))
        std::cout << "load Basicrom Success..." << std::endl;
    if(cpu.LoadRom(0xC0000, "./videorom.bin"))
        std::cout << "load Videorom Success..." << std::endl;

    //  gettimeofday(&tv, NULL);
    //  start_time = tv.tv_sec * 1000000 + tv.tv_usec;

    /*run CPU*/
    pthread_create(&runthread, NULL, RunThread, (void *)(&cpu));

    /*start SDL event listen*/
    input_arg.cpu_arg = &cpu;
    input_arg.keyboard_arg = &keyboard;
    pthread_create(&inputthread, NULL, InputThread, (void *)(&input_arg));

    /*start console*/
    if(commandflag == 1)
    {
        console_arg.cpu_arg = &cpu;
        console_arg.fd = &fd1;
        console_arg.Disk_handle = &Disk_handle;
        pthread_create(&consolethread, NULL, ConsoleThread, (void *)(&console_arg));
    }
    /*debug*/
    //  if(a == 1)
    //  {
    //      printf("now change disk!\n");
    //      Disk *fd2 = new Disk("./dos2.flp", 0);
    //      fd2->setcpu(&cpu);
    //      Disk_handle.eject_disk(0, &fd1);
    //      Disk_handle.insert_disk(0, fd2);
    //      printf("change finished!\n");
    //  }
    /**/
    pthread_join(runthread, NULL);

    // gettimeofday(&tv, NULL);
    // end_time = tv.tv_sec * 1000000 + tv.tv_usec;
    //printf("\n\nExecute %llu Opcode in %lf Seconds.\nAverage:%llu Opcode/S.\n", cpu.GetCount(), (end_time - start_time) / 1000000.0, (uint64_t)(cpu.GetCount() / ((end_time - start_time) / 1000000.0)));
    std::cout << "\n" << std::endl;
    return 0;
}
