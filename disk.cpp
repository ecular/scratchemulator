#include "disk.h"
#include "cpu.h"

Disk::Disk(string filename, uint8_t driver_num)
{
    read_write.open(filename.c_str(), ios::in | ios::out | ios::binary); //open file
    if(!read_write.is_open())
    {
        //cout<<"input file open error"<<endl;
        return;
    }
    read_write.seekg(0, ios::end);
    filesize = read_write.tellg();
    if(driver_num >= 0x80) //it's a hard disk
    {
        sectors = 63;
        heads = 8;
        cylinders = filesize / (sectors * heads * 512);
    }
    else//it's a floppy disk (just for 1.44MB)
    {
        sectors = 18;
        heads = 2;
        cylinders = 80;
    }
}

void Disk::setcpu(Cpu *arg)
{
    cpu = arg;
}

Disk::~Disk()
{
    read_write.close();
}

void Disk::writedisk(uint16_t sector_count, uint16_t cylinder, uint16_t sector, uint8_t head, uint8_t dirver_num, uint16_t mem_seg, uint16_t mem_offset)
{
    char sector_buffer[512];
    uint8_t buffer_offset;
    uint32_t fileoffset = ((cylinder * heads + head) * sectors + sector - 1) * 512;
    uint32_t mem_address = (mem_seg << 4) + mem_offset;
    read_write.seekg(fileoffset, ios::beg);
    for(int current_sector = 0; current_sector < sector_count; ++current_sector)
    {
        buffer_offset = 0;
        while(buffer_offset < 512)
        {
            sector_buffer[buffer_offset++] = cpu->ReadRam8(mem_address++);
        }
        read_write.write(sector_buffer, 512);
        read_write.sync();
    }
    cpu->SetAL(sector_count);
    cpu->SetAH(0);
    cpu->SetFlag(cpu->GetFlag() & 0xFFFE);//CF = 0
}

void Disk::readdisk(uint16_t sector_count, uint16_t cylinder, uint16_t sector, uint8_t head, uint8_t dirver_num, uint16_t mem_seg, uint16_t mem_offset)
{
    char sector_buffer[512];
    uint8_t buffer_offset;
    uint32_t fileoffset = ((cylinder * heads + head) * sectors + sector - 1) * 512;
    uint32_t mem_address = (mem_seg << 4) + mem_offset;
    read_write.seekg(fileoffset, ios::beg);
    for(int current_sector = 0; current_sector < sector_count; ++current_sector)
    {
        read_write.read(sector_buffer, 512);
        read_write.sync();
        buffer_offset = 0;
        while(buffer_offset < 512)
        {
            cpu->WriteRam8(mem_address++, sector_buffer[buffer_offset++]);
        }
    }
    cpu->SetAL(sector_count);
    cpu->SetAH(0);
    cpu->SetFlag(cpu->GetFlag() & 0xFFFE);//CF = 0
}

