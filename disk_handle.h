#ifndef EMU_DISK_HANDLE_H
#define EMU_DISK_HANDLE_H

#include <map>
#include "cpu.h"
#include "disk.h"

class disk_handle {
private:
    Cpu &cpu;
    uint8_t hard_count, floppy_count;
    map<uint8_t, Disk *> disk_map;
public:
    void insert_disk(uint8_t , Disk *);
    disk_handle(Cpu &);
    void disk_operator(void);
};

#endif
