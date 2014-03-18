#ifndef EMU_DISK_H
#define EMU_DISK_H
#include <stdint.h>
#include <fstream>
#include <string>

#include "cpu.h"

using namespace std;

class Disk {
    friend class diskOperator;
private:
    void writedisk(uint16_t sector_count, uint16_t cylinder, uint16_t sector, uint8_t head, uint8_t dirver_num, uint16_t mem_seg, uint16_t mem_offset);
    void readdisk(uint16_t sector_count, uint16_t cylinder, uint16_t sector, uint8_t head, uint8_t dirver_num, uint16_t mem_seg, uint16_t mem_offset);
    fstream read_write;
    /*disk property*/
    uint32_t filesize;
    uint16_t cylinders;
    uint8_t sectors;
    uint8_t heads;

    Cpu &cpu;
public:
    Disk(Cpu &, string, uint8_t);
    ~Disk();
};

#endif
