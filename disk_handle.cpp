#include "disk_handle.h"
#include "cpu.h"
#include <algorithm>

disk_handle::disk_handle()
{
    hard_count = 0;
    floppy_count = 0;
}

void disk_handle::insert_disk(uint8_t disk_num, Disk *disk)
{
    vector<uint8_t>::iterator found;
    if(disk_num < 0x80)//it is a floppy disk
        floppy_count++;
    else//it is a hard disk
        hard_count++;

    disk_map.insert(map<uint8_t, Disk *>::value_type(disk_num, disk));

    found = find(inserted_disknum.begin(), inserted_disknum.end(), disk_num);
    if(found != inserted_disknum.end())
    {
        ;//do nothing
    }
    else
        inserted_disknum.push_back(disk_num);

}

void disk_handle::eject_disk(uint8_t disk_num, Disk *disk)
{
    vector<uint8_t>::iterator found;
    for(map<uint8_t, Disk *>::iterator i = disk_map.begin(); i != disk_map.end(); i++)
    {
        if(i->first == disk_num && i->second == disk)
        {
            disk_map.erase(i);
            break;
        }
    }

    found = find(inserted_disknum.begin(), inserted_disknum.end(), disk_num);
    if(found != inserted_disknum.end())
    {
        inserted_disknum.erase(found);
    }
}


void disk_handle::disk_operator()
{
    static uint8_t lastdiskah[256], lastdiskcf[256];
    switch(cpu->GetAH())
    {
    case(0): /*Reset Disk Drive*/
    {
//       printf("0\n");
        cpu->SetFlag(cpu->GetFlag() & 0xFFFE);//CF = 0
        break;
    }

    case(1): /*Get Status of Last Drive Operation*/
    {
        //     printf("1\n");
        cpu->SetAH(lastdiskah[cpu->GetDL()]);
        if(lastdiskcf[cpu->GetDL()] == 0)
            cpu->SetFlag(cpu->GetFlag() & 0xFFFE);//CF = 0
        else
            cpu->SetFlag(cpu->GetFlag() | 0x1);//CF = 1
        return;
    }
    case(2): /*Read Sectors From Drive*/
    {
        vector<uint8_t>::iterator found;
        found = find(inserted_disknum.begin(), inserted_disknum.end(), cpu->GetDL());
        if(found != inserted_disknum.end())
        {
            //  printf("2\n");
            //  printf("al %x ch %x cl %x dh %x dl %x es %x bx %x\n", cpu->GetAL(), cpu->GetCH(), cpu->GetCL(), cpu->GetDH(), cpu->GetDL(), cpu->GetES(), cpu->GetBX());
            //  fflush(stdout);
            disk_map[cpu->GetDL()]->readdisk(cpu->GetAL(), cpu->GetCL() / 64 * 256 + cpu->GetCH(), cpu->GetCL() & 0x3F, cpu->GetDH(), cpu->GetDL(), cpu->GetES(), cpu->GetBX());
            cpu->SetFlag(cpu->GetFlag() & 0xFFFE);//CF = 0
            cpu->SetAH(0);
        }
        else
        {
            cpu->SetFlag(cpu->GetFlag() | 0x1);//CF = 1
            cpu->SetAH(0x1);
        }
        break;
    }
    case(3): /*Write Sectors To Drive*/
    {
        vector<uint8_t>::iterator found;
        found = find(inserted_disknum.begin(), inserted_disknum.end(), cpu->GetDL());
        if(found != inserted_disknum.end())
        {
            //       printf("3\n");
            disk_map[cpu->GetDL()]->writedisk(cpu->GetAL(), cpu->GetCL() / 64 * 256 + cpu->GetCH(), cpu->GetCL() & 0x3F, cpu->GetDH(), cpu->GetDL(), cpu->GetES(), cpu->GetBX());
            cpu->SetFlag(cpu->GetFlag() & 0xFFFE);//CF = 0
            cpu->SetAH(0);
        }
        else
        {
            cpu->SetFlag(cpu->GetFlag() | 0x1);//CF = 1
            cpu->SetAH(0x1);
        }
        break;
    }
    case(4): /*Verify Sectors From Drive*/
    case(5): /*Format Track*/
    {
        cpu->SetFlag(cpu->GetFlag() & 0xFFFE);//CF = 0
        cpu->SetAH(0);
        break;
    }
    case(8): /*Read Drive Parameters*/
    {
        vector<uint8_t>::iterator found;
        found = find(inserted_disknum.begin(), inserted_disknum.end(), cpu->GetDL());
        if(found != inserted_disknum.end())
        {
            cpu->SetFlag(cpu->GetFlag() & 0xFFFE);//CF = 0
            cpu->SetAH(0);//Return Code
            cpu->SetDH(disk_map[cpu->GetDL()]->heads - 1);//heads_index = heads_count - 1 (start with 0)
            cpu->SetCH(((disk_map[cpu->GetDL()]->cylinders - 1) & 0x3FF) >> 2);//cylinders_index = cylinders - 1 (start with 0)
            cpu->SetCL((((disk_map[cpu->GetDL()]->cylinders - 1) & 0x3) << 6) + (disk_map[cpu->GetDL()]->sectors & 0x3F));//setctor_index = sectors (start with 1)
            if(cpu->GetDL() < 0x80)
            {
                cpu->SetBL(4);
                cpu->SetDL(2);
            }
            else
            {
                cpu->SetDL(hard_count);//hard disk count
            }
        }
        else
        {
            cpu->SetFlag(cpu->GetFlag() | 0x1);//CF = 1
            cpu->SetAH(0xAA);
        }
        break;
    }
    default:
        cpu->SetFlag(cpu->GetFlag() | 0x1);//CF = 1
    }
    lastdiskah[cpu->GetDL()] = cpu->GetAH();
    if(cpu->GetFlag() & 0x1) //CF = 1
        lastdiskcf[cpu->GetDL()] = 1;
    else
        lastdiskcf[cpu->GetDL()] = 0;
}

void disk_handle::setcpu(Cpu *arg)
{
    cpu = arg;
}
