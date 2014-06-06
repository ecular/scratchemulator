#ifndef EMU_DISK_HANDLE_H
#define EMU_DISK_HANDLE_H

#include <map>
#include <vector>
#include "disk.h"

class Cpu;

class disk_handle {
    friend class Cpu;
private:
    Cpu *cpu;
    map<uint8_t, Disk *> disk_map;
    vector<uint8_t> inserted_disknum;
public:
    uint8_t hard_count, floppy_count;
    void insert_disk(uint8_t , Disk *);
    void eject_disk(uint8_t , Disk *);
    disk_handle();
    void disk_operator(void);
    void setcpu(Cpu *);
};

#endif
