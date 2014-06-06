#ifndef EMU_CPU_H_
#define EMU_CPU_H_

#include <stdint.h>
#include <string>

#include "port_handle.h"
#include "8259a.h"
#include "8253.h"
#include "video.h"
#include "disk_handle.h"
#include "disk.h"
#include "timer.h"

class Cpu {
public:
    //Cpu(port_handle &, Interrupt_Controller_8259a &, Interval_Timer_8253 &, Video &, disk_handle &);
    Cpu();
    /*set device*/
    void setports_operate(port_handle *);
    void seti8259a(Interrupt_Controller_8259a *);
    void seti8253(Interval_Timer_8253 *);
    void setvideo(Video *);
    void setDisk_handle(disk_handle *);
    void settimer(timer *);
    /*set reg*/
    void SetAX(uint16_t value);
    void SetAH(uint8_t value);
    void SetAL(uint8_t value);
    void SetBX(uint16_t value);
    void SetBH(uint8_t value);
    void SetBL(uint8_t value);
    void SetCX(uint16_t value);
    void SetCH(uint8_t value);
    void SetCL(uint8_t value);
    void SetDX(uint16_t value);
    void SetDH(uint8_t value);
    void SetDL(uint8_t value);
    void SetSP(uint16_t value);
    void SetBP(uint16_t value);
    void SetSI(uint16_t value);
    void SetDI(uint16_t value);

    void SetIP(uint16_t value);
    void SetFlag(uint16_t value);

    void SetCS(uint16_t value);
    void SetDS(uint16_t value);
    void SetSS(uint16_t value);
    void SetES(uint16_t value);


    /*get reg*/
    uint16_t GetAX();
    uint8_t GetAH();
    uint8_t GetAL();
    uint16_t GetBX();
    uint8_t GetBH();
    uint8_t GetBL();
    uint16_t GetCX();
    uint8_t GetCH();
    uint8_t GetCL();
    uint16_t GetDX();
    uint8_t GetDH();
    uint8_t GetDL();
    uint16_t GetSP();
    uint16_t GetBP();
    uint16_t GetSI();
    uint16_t GetDI();

    uint16_t GetIP();
    uint16_t GetFlag();

    uint16_t GetCS();
    uint16_t GetDS();
    uint16_t GetSS();
    uint16_t GetES();

    void IPStep(unsigned int);

    /*this two functions only used in Exec() */
    uint8_t ReadData8InExe();
    uint16_t ReadData16InExe();


    /*write ram*/
    void WriteRam8(unsigned int location, uint8_t value);
    void WriteRam16(unsigned int location, uint16_t value);

    /*read ram*/
    uint8_t ReadRam8(unsigned int location);
    uint16_t ReadRam16(unsigned int location);

    /*some basic operates*/
    void Push(uint16_t);
    uint16_t Pop();

    int Init(unsigned int ram_size);
    int Reset();
    void Exec(uint32_t);

    /*load BOIS*/
    bool LoadBIOS(string);
    /*load rom*/
    bool LoadRom(uint32_t, string);

    /*change/get machine run status*/
    bool GetStatus(void);
    void SetStatus(bool);

    /*change boot device*/
    void SetBootDevice(uint16_t);

    /*get opcode conut*/
    uint64_t GetCount(void);

    uint8_t *ram;
private:

    uint8_t *CalculateReg8(uint8_t mod_byte);
    uint16_t *CalculateReg16(uint8_t mod_byte);
    uint16_t *CalculateSeg16(uint8_t mod_byte);
    uint8_t *CalculateRM(uint8_t mod_byte, uint8_t opcode);
    void Intcall(uint8_t);//for interupt call
    /*for I/O port operate*/
    uint8_t read8_from_port(uint8_t);
    uint16_t read16_from_port(uint8_t);
    void write8_to_port(uint8_t, uint8_t);
    void write16_to_port(uint8_t, uint16_t);

    uint16_t universal_reg_ax, universal_reg_bx, universal_reg_cx, universal_reg_dx,
             universal_reg_sp, universal_reg_bp, universal_reg_si, universal_reg_di;
    uint8_t *universal_reg_ah, *universal_reg_al;
    uint8_t *universal_reg_bh, *universal_reg_bl;
    uint8_t *universal_reg_ch, *universal_reg_cl;
    uint8_t *universal_reg_dh, *universal_reg_dl;
    uint16_t control_reg_ip, control_reg_flag;
    uint16_t seg_reg_cs, seg_reg_ds, seg_reg_ss, seg_reg_es;
    uint16_t *seg_reg_replace_ds, *seg_reg_replace_ss;
    uint8_t if_flag, tf_flag;
    bool halt;
    uint16_t boot_device;
    /*other devices*/
    port_handle *ports_operate;
    Interrupt_Controller_8259a *i8259a;
    Interval_Timer_8253 *i8253;
    Video *video;
    disk_handle *Disk_handle;
    timer *timing;

    /*debug fuction*/
    void printdebug(uint8_t, uint32_t, uint32_t);
    void change_print_8bit(uint8_t *p);
    void change_print_16bit(uint16_t *p);
    void printf_my(uint16_t);
    uint64_t MAX;
    uint64_t MIN;
    uint64_t count_code;

};

#endif
