#include "disk_handle.h"

disk_handle::disk_handle(Cpu &cpu_arg): cpu(cpu_arg)
{
    hard_count = 0;
    floppy_count = 0;
}

void disk_handle::insert_disk(uint8_t disk_num, Disk *disk)
{
    if(disk_num < 0x80)
        floppy_count++;
    else
        hard_count++;
    disk_map.insert(map<uint8_t, Disk *>::value_type(disk_num, disk));
}

void disk_handle::disk_operator()
{
    switch(cpu.GetAH())
    {
    case(0): /*Reset Disk Drive*/
    {
        cpu.SetFlag(cpu.GetFlag() & 0xFFFE);//CF = 0
        break;
    }

    case(1): /*Get Status of Last Drive Operation*/
    {
        cpu.SetAH(disk_map[cpu.GetDL()]->lastAH);
        if(disk_map[cpu.GetDL()]->lastCF == 0)
            cpu.SetFlag(cpu.GetFlag() & 0xFFFE);//CF = 0
        else
            cpu.SetFlag(cpu.GetFlag() | 0x1);//CF = 1
        return;
    }
    case(2): /*Read Sectors From Drive*/
    {
        disk_map[cpu.GetDL()]->readdisk(cpu.GetAL(), cpu.GetCL() / 64 * 256 + cpu.GetCH(), cpu.GetCL() & 0x3F, cpu.GetDH(), cpu.GetDL(), cpu.GetES(), cpu.GetBX());
        break;
    }
    case(3): /*Write Sectors To Drive*/
    {
        disk_map[cpu.GetDL()]->writedisk(cpu.GetAL(), cpu.GetCL() / 64 * 256 + cpu.GetCH(), cpu.GetCL() & 0x3F, cpu.GetDH(), cpu.GetDL(), cpu.GetES(), cpu.GetBX());
        break;
    }
    case(4): /*Verify Sectors From Drive*/
    case(5): /*Format Track*/
    {
        cpu.SetFlag(cpu.GetFlag() & 0xFFFE);//CF = 0
        cpu.SetAH(0);
    }
    case(8): /*Read Drive Parameters*/
    {
        cpu.SetFlag(cpu.GetFlag() & 0xFFFE);//CF = 0
        cpu.SetAH(0);//Return Code
        cpu.SetDH(disk_map[cpu.GetDL()]->heads - 1);//heads_index = heads_count - 1 (start with 0)
        cpu.SetCH(((disk_map[cpu.GetDL()]->cylinders - 1) & 0x3FF) >> 2);//cylinders_index = cylinders - 1 (start with 0)
        cpu.SetCL((((disk_map[cpu.GetDL()]->cylinders - 1) & 0x3) << 6) + (disk_map[cpu.GetDL()]->sectors & 0x3F));//setctor_index = sectors (start with 1)
        if(cpu.GetDL() < 0x80)
        {
            cpu.SetBL(4);
            cpu.SetDL(2);
        }
        else
        {
            cpu.SetDL(hard_count);//hard disk count
        }
        break;
    }
    default:
        cpu.SetFlag(cpu.GetFlag() | 0x1);//CF = 1
    }
    disk_map[cpu.GetDL()]->lastAH = cpu.GetAH();
    if(cpu.GetFlag() & 0x1) //CF = 1
        disk_map[cpu.GetDL()]->lastCF = 1;
    else
        disk_map[cpu.GetDL()]->lastCF = 0;
}
