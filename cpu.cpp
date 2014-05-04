#define CPU_80186

#include "cpu.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "8259a.h"
#include "8253.h"

Cpu::Cpu()
{
    universal_reg_al = reinterpret_cast<uint8_t *>(&universal_reg_ax);
    universal_reg_ah = universal_reg_al + 1;
    universal_reg_bl = reinterpret_cast<uint8_t *>(&universal_reg_bx);
    universal_reg_bh = universal_reg_bl + 1;
    universal_reg_cl = reinterpret_cast<uint8_t *>(&universal_reg_cx);
    universal_reg_ch = universal_reg_cl + 1;
    universal_reg_dl = reinterpret_cast<uint8_t *>(&universal_reg_dx);
    universal_reg_dh = universal_reg_dl + 1;
}

/*set device*/
void Cpu::setports_operate(port_handle *arg)
{
    ports_operate = arg;
}
void Cpu::seti8259a(Interrupt_Controller_8259a *arg)
{
    i8259a = arg;
}
void Cpu::seti8253(Interval_Timer_8253 *arg)
{
    i8253 = arg;
}
void Cpu::setvideo(Video *arg)
{
    video = arg;
}
void Cpu::setDisk_handle(disk_handle *arg)
{
    Disk_handle = arg;
}
void Cpu::settimer(timer *arg)
{
    timing = arg;
}

/*set reg*/
inline void Cpu::SetAX(uint16_t value)
{
    universal_reg_ax = value & 0xFFFF;
}
inline void Cpu::SetAH(uint8_t value)
{
    //universal_reg_ax = (universal_reg_ax & 0x00FF) | ((value & 0x00FF) << 8);
    *universal_reg_ah = value & 0xFF;
}
inline void Cpu::SetAL(uint8_t value)
{
    //   universal_reg_ax = (universal_reg_ax & 0xFF00) | (value & 0x00FF);
    *universal_reg_al = value & 0xFF;
}

inline void Cpu::SetBX(uint16_t value)
{
    universal_reg_bx = value & 0xFFFF;
}
inline void Cpu::SetBH(uint8_t value)
{
    //universal_reg_bx = (universal_reg_bx & 0x00FF) | ((value & 0x00FF) << 8);
    *universal_reg_bh = value & 0xFF;
}
inline void Cpu::SetBL(uint8_t value)
{
    //universal_reg_bx = (universal_reg_bx & 0xFF00) | (value & 0x00FF);
    *universal_reg_bl = value & 0xFF;
}

inline void Cpu::SetCX(uint16_t value)
{
    universal_reg_cx = value & 0xFFFF;
}
inline void Cpu::SetCH(uint8_t value)
{
    //universal_reg_cx = (universal_reg_cx & 0x00FF) | ((value & 0x00FF) << 8);
    *universal_reg_ch = value & 0xFF;
}
inline void Cpu::SetCL(uint8_t value)
{
    //universal_reg_cx = (universal_reg_cx & 0xFF00) | (value & 0x00FF);
    *universal_reg_cl = value & 0xFF;
}

inline void Cpu::SetDX(uint16_t value)
{
    universal_reg_dx = value & 0xFFFF;
}
inline void Cpu::SetDH(uint8_t value)
{
    //universal_reg_dx = (universal_reg_dx & 0x00FF) | ((value & 0x00FF) << 8);
    *universal_reg_dh = value & 0xFF;
}
inline void Cpu::SetDL(uint8_t value)
{
    //universal_reg_dx = (universal_reg_dx & 0xFF00) | (value & 0x00FF);
    *universal_reg_dl = value & 0xFF;
}

inline void Cpu::SetSP(uint16_t value)
{
    universal_reg_sp = value & 0xFFFF;
}

inline void Cpu::SetBP(uint16_t value)
{
    universal_reg_bp = value & 0xFFFF;
}

inline void Cpu::SetSI(uint16_t value)
{
    universal_reg_si = value & 0xFFFF;
}

inline void Cpu::SetDI(uint16_t value)
{
    universal_reg_di = value & 0xFFFF;
}

inline void Cpu::SetIP(uint16_t value)
{
    control_reg_ip = value & 0xFFFF;
}

inline void Cpu::SetFlag(uint16_t value)
{
    control_reg_flag = value & 0xFFFF;
}

inline void Cpu::SetCS(uint16_t value)
{
    seg_reg_cs = value & 0xFFFF;
}

inline void Cpu::SetDS(uint16_t value)
{
    seg_reg_ds = value & 0xFFFF;
}

inline void Cpu::SetSS(uint16_t value)
{
    seg_reg_ss = value & 0xFFFF;
}

inline void Cpu::SetES(uint16_t value)
{
    seg_reg_es = value & 0xFFFF;
}

/*get reg*/
inline uint16_t Cpu::GetAX()
{
    return universal_reg_ax;
}
inline uint8_t Cpu::GetAH()
{
    //return universal_reg_ax >> 8;
    return *universal_reg_ah;
}
inline uint8_t Cpu::GetAL()
{
    //return static_cast<uint8_t>(universal_reg_ax);
    return *universal_reg_al;
}

inline uint16_t Cpu::GetBX()
{
    return universal_reg_bx;
}
inline uint8_t Cpu::GetBH()
{
    return universal_reg_bx >> 8;
}
inline uint8_t Cpu::GetBL()
{
    return static_cast<uint8_t>(universal_reg_bx);
}

inline uint16_t Cpu::GetCX()
{
    return universal_reg_cx;
}
inline uint8_t Cpu::GetCH()
{
    return universal_reg_cx >> 8;
}
inline uint8_t Cpu::GetCL()
{
    return static_cast<uint8_t>(universal_reg_cx);
}

inline uint16_t Cpu::GetDX()
{
    return universal_reg_dx;
}
inline uint8_t Cpu::GetDH()
{
    return universal_reg_dx >> 8;
}
inline uint8_t Cpu::GetDL()
{
    return static_cast<uint8_t>(universal_reg_dx);
}

inline uint16_t Cpu::GetSP()
{
    return universal_reg_sp;
}

inline uint16_t Cpu::GetBP()
{
    return universal_reg_bp;
}

inline uint16_t Cpu::GetSI()
{
    return universal_reg_si;
}

inline uint16_t Cpu::GetDI()
{
    return universal_reg_di;
}

inline uint16_t Cpu::GetIP()
{
    return control_reg_ip;
}

inline uint16_t Cpu::GetFlag()
{
    return control_reg_flag;
}

inline uint16_t Cpu::GetCS()
{
    return seg_reg_cs;
}

inline uint16_t Cpu::GetDS()
{
    return seg_reg_ds;
}

inline uint16_t Cpu::GetSS()
{
    return seg_reg_ss;
}

inline uint16_t Cpu::GetES()
{
    return seg_reg_es;
}

inline void Cpu::IPStep(unsigned int steps)
{
    control_reg_ip = control_reg_ip + steps;
}

inline uint8_t Cpu::ReadData8InExe()
{
    uint8_t data = ReadRam8((seg_reg_cs << 4) + control_reg_ip); //get 1byte form Ram
    IPStep(1);
    return data;
}
inline uint16_t Cpu::ReadData16InExe()
{
    uint8_t data_low = ReadData8InExe();
    uint8_t data_high = ReadData8InExe();
    return (static_cast<uint16_t>(data_high) << 8) + data_low;
}

/*write ram*/
inline void Cpu::WriteRam8(uint32_t location, uint8_t value)
{
    location = location & 0xFFFFF;
    if((location >= 0xC0000 && location < 0xC8000) || location >= 0xF6000) /*write to BIOS*/
        return;
    if(location >= 0xA0000 && location <= 0xBFFFF)/*write to video memory*/
    {
        if(video->Video_Mode != 0x0D && video->Video_Mode != 0x10 && video->Video_Mode != 0x12)
        {
            if(video->Video_Mode != 0x13 || (video->Sequence_Reg[4] & 0x6) == 0)
                ram[location] = value & 0x00FF;
        }
        video->screen_update_flag = 1;
    }
    else
        ram[location] = value & 0x00FF;
}

inline void Cpu::WriteRam16(uint32_t location, uint16_t value)
{
    WriteRam8(location++, value & 0x00FF);
    WriteRam8(location, value >> 8);
}

/*read ram*/
inline uint8_t Cpu::ReadRam8(uint32_t location)
{
    location = location & 0xFFFFF;
    if(location >= 0xA0000 && location <= 0xBFFFF)
    {
        if(video->Video_Mode == 0x0D  || video->Video_Mode == 0x0E || video->Video_Mode == 0x10 || video->Video_Mode == 0x12)
            return video->read_video_memory(location);
        if(video->Video_Mode != 0x0D && video->Video_Mode != 0x12 && video->Video_Mode != 0x13)
            return ram[location];
        if((video->Sequence_Reg[4] & 0x6) == 0)
            return ram[location];
    }
    ram[0x410] = 0x41;/*we have an EGA?VGA card installed*/
    ram[0x475] = Disk_handle->hard_count;
    return ram[location];
}

inline uint16_t Cpu::ReadRam16(uint32_t location)
{
    return (ReadRam8(location) | (ReadRam8(location + 1) * 256));
}

/*calculate Mod byte*/
uint8_t *Cpu::CalculateReg8(uint8_t mod_byte)
{
    switch((mod_byte >> 3) & 0x7)
    {
    case(0x0):
        return universal_reg_al;
    case(0x1):
        return universal_reg_cl;
    case(0x2):
        return universal_reg_dl;
    case(0x3):
        return universal_reg_bl;
    case(0x4):
        return universal_reg_ah;
    case(0x5):
        return universal_reg_ch;
    case(0x6):
        return universal_reg_dh;
    case(0x7):
        return universal_reg_bh;
    }
    return NULL;//will never be here
}

uint16_t *Cpu::CalculateReg16(uint8_t mod_byte)
{
    switch((mod_byte >> 3) & 0x7)
    {
    case(0x0):
        return &universal_reg_ax;
    case(0x1):
        return &universal_reg_cx;
    case(0x2):
        return &universal_reg_dx;
    case(0x3):
        return &universal_reg_bx;
    case(0x4):
        return &universal_reg_sp;
    case(0x5):
        return &universal_reg_bp;
    case(0x6):
        return &universal_reg_si;
    case(0x7):
        return &universal_reg_di;
    }
    return NULL;//will never be here
}

uint16_t *Cpu::CalculateSeg16(uint8_t mod_byte)
{
    switch((mod_byte >> 3) & 0x3)
    {
    case(0x0):
        return &seg_reg_es;
    case(0x1):
        return &seg_reg_cs;
    case(0x2):
        return &seg_reg_ss;
    case(0x3):
        return &seg_reg_ds;
    }
    return NULL;//will never be here
}

uint8_t *Cpu::CalculateRM(uint8_t mod_byte, uint8_t opcode)
{
    uint8_t rm = mod_byte & 0x7;
    uint8_t mod_bit = (mod_byte >> 6) & 0x3;
    opcode = opcode & 0x1;//W bit
    switch(mod_bit)
    {
    case(0x0):
        switch(rm)
        {
        case(0x00):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx + universal_reg_si];
        case(0x01):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx + universal_reg_di];
        case(0x02):
            return &ram[(*seg_reg_replace_ss << 4) + universal_reg_bp + universal_reg_si];
        case(0x03):
            return &ram[(*seg_reg_replace_ss << 4) + universal_reg_bp + universal_reg_di];
        case(0x04):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_si];
        case(0x05):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_di];
        case(0x06):
            return &ram[(*seg_reg_replace_ds << 4) + ReadData16InExe()];
        case(0x07):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx];
        }

    case(0x1):
        switch(rm)
        {
        case(0x00):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx + universal_reg_si + ReadData8InExe()];
        case(0x01):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx + universal_reg_di + ReadData8InExe()];
        case(0x02):
            return &ram[(*seg_reg_replace_ss << 4) + universal_reg_bp + universal_reg_si + ReadData8InExe()];
        case(0x03):
            return &ram[(*seg_reg_replace_ss << 4) + universal_reg_bp + universal_reg_di + ReadData8InExe()];
        case(0x04):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_si + ReadData8InExe()];
        case(0x05):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_di + ReadData8InExe()];
        case(0x06):
            return &ram[(*seg_reg_replace_ss << 4) + universal_reg_bp + ReadData8InExe()];
        case(0x07):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx + ReadData8InExe()];
        }

    case(0x2):
        switch(rm)
        {
        case(0x00):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx + universal_reg_si + ReadData16InExe()];
        case(0x01):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx + universal_reg_di + ReadData16InExe()];
        case(0x02):
            return &ram[(*seg_reg_replace_ss << 4) + universal_reg_bp + universal_reg_si + ReadData16InExe()];
        case(0x03):
            return &ram[(*seg_reg_replace_ss << 4) + universal_reg_bp + universal_reg_di + ReadData16InExe()];
        case(0x04):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_si + ReadData16InExe()];
        case(0x05):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_di + ReadData16InExe()];
        case(0x06):
            return &ram[(*seg_reg_replace_ss << 4) + universal_reg_bp + ReadData16InExe()];
        case(0x07):
            return &ram[(*seg_reg_replace_ds << 4) + universal_reg_bx + ReadData16InExe()];
        }
    case(0x3):
        if(opcode == 0x0)
        {
            switch(rm)
            {
            case(0x0):
                return universal_reg_al;
            case(0x1):
                return universal_reg_cl;
            case(0x2):
                return universal_reg_dl;
            case(0x3):
                return universal_reg_bl;
            case(0x4):
                return universal_reg_ah;
            case(0x5):
                return universal_reg_ch;
            case(0x6):
                return universal_reg_dh;
            case(0x7):
                return universal_reg_bh;
            }
        }
        else//W = 1
        {
            switch(rm)
            {
            case(0x0):
                return reinterpret_cast<uint8_t *>(&universal_reg_ax);
            case(0x1):
                return reinterpret_cast<uint8_t *>(&universal_reg_cx);
            case(0x2):
                return reinterpret_cast<uint8_t *>(&universal_reg_dx);
            case(0x3):
                return reinterpret_cast<uint8_t *>(&universal_reg_bx);
            case(0x4):
                return reinterpret_cast<uint8_t *>(&universal_reg_sp);
            case(0x5):
                return reinterpret_cast<uint8_t *>(&universal_reg_bp);
            case(0x6):
                return reinterpret_cast<uint8_t *>(&universal_reg_si);
            case(0x7):
                return reinterpret_cast<uint8_t *>(&universal_reg_di);
            }
        }
    }
    return NULL;//will never be here

}
/*some basic operate*/
inline void Cpu::Push(uint16_t data)
{
    universal_reg_sp = universal_reg_sp - 2;
    WriteRam16(seg_reg_ss * 16 + universal_reg_sp, data);
}

inline uint16_t Cpu::Pop()
{
    uint16_t tmp_data = ReadRam16(seg_reg_ss * 16 + universal_reg_sp);
    universal_reg_sp = universal_reg_sp + 2;
    return tmp_data;
}

int Cpu::Init(unsigned int ram_size)
{
    count_code = 0;
    ram = new uint8_t[ram_size];
    memset(ram, 0x0, ram_size);
    // memset(ram + 0x8000, ' ', 80 * 25);
    Reset();
    halt = 0;
    return 0;
}

int Cpu::Reset()
{
    control_reg_flag = 0x0;
    control_reg_ip = 0x0;
    seg_reg_cs = 0xFFFF;
    seg_reg_ds = 0x0;
    seg_reg_ss = 0x0;
    seg_reg_es = 0x0;
    if_flag = 0;
    tf_flag = 0;
    return 0;
}

void Cpu::Intcall(uint8_t int_num)
{

    // fprintf(stdout,"int call1 %x\n",int_num);
    // fprintf(stdout,"int call2 %x\n",int_num);
    if(count_code == 586751 || count_code == 588913 || count_code == 590112)
        printf("intcall at %ld num %d\n", count_code, int_num);

    uint16_t oldax;
    switch(int_num)
    {
    case(0x10):
    {
        video->screen_update_flag = 1;
        if(GetAH() == 0x00 || GetAH() == 0x10)
        {
            oldax = GetAX();
            video->interrupt();
            SetAX(oldax);
            if(GetAH() == 0x10 || video->Video_Mode == 0x09)
                return;
        }
        break;
    }
    case(0x19):
    {
        *universal_reg_dl = 0x0;
        Disk_handle->disk_map[GetDL()]->readdisk(1, 0, 1, 0, *universal_reg_dl, 0x07C0, 0x0000);
        seg_reg_cs = 0x0000;
        control_reg_ip = 0x7C00;
        return;
    }

    case(0x13):
    case(0xFD):
        Disk_handle->disk_operator();
        return;

    }
    if(if_flag == 1)//PUSH Flash
        Push(control_reg_flag | 0x0200);
    else
        Push(control_reg_flag & 0xFDFF);

    if_flag = 0;//set IF = 0
    control_reg_flag = control_reg_flag & 0xFEFF;//set TF = 0

    Push(seg_reg_cs);//push cs
    Push(control_reg_ip);//push ip
    seg_reg_cs = ReadRam16((0 << 4) + int_num * 4 + 2);//read cs from Interrupt vector table
    control_reg_ip = ReadRam16((0 << 4) + int_num * 4);//read ip from INterrupt vector table
}

uint8_t Cpu::read8_from_port(uint8_t port_num)
{
    return ports_operate->port_handle_read8(port_num);
}

uint16_t Cpu::read16_from_port(uint8_t port_num)
{
    return ports_operate->port_handle_read16(port_num);
}

void Cpu::write8_to_port(uint8_t port_num, uint8_t value)
{
    return ports_operate->port_handle_write8(port_num, value);
}

void Cpu::write16_to_port(uint8_t port_num, uint16_t value)
{
    ports_operate->port_handle_write16(port_num, value);
}

void Cpu::Exec(uint32_t loops)
{
    uint8_t opcode;
    uint8_t mod_byte;
    uint8_t *opt1_8bit, *opt2_8bit;
    uint16_t *opt1_16bit, *opt2_16bit;
    static uint16_t firstip;
    uint8_t continue_check = 1;
    static int32_t Instruction_counts = 0;
    static uint8_t one_step_mode = 0;
    FILE *fp = fopen("/home/ecular/opcodemy.txt", "at");
    /*debug*/
    uint32_t ip_tmp;
    /**/
#ifdef CPU_80186
    uint8_t rep = 0;
#endif

    for(uint32_t loopscounts = 0; loopscounts < loops; ++loopscounts, ++Instruction_counts)
    {
        seg_reg_replace_ds = &seg_reg_ds;
        seg_reg_replace_ss = &seg_reg_ss;

        if((Instruction_counts & 15) == 0)
            timing->timer_tick();

        /*one step mode*/
        if(one_step_mode)
            Intcall(1);//one step interrupt

        /*check tf,before  one opcode*/
        if(tf_flag)
            one_step_mode = 1;
        else
            one_step_mode = 0;

        /*get int_num from 8259a*/
        if(if_flag && (i8259a->IRR & (~i8259a->IMR)))
            Intcall(i8259a->send_int_cpu());

        /*read a opcode from Memory via CS:IP*/
        firstip = control_reg_ip;
        ip_tmp = (seg_reg_cs << 4) + control_reg_ip;
        opcode = ReadData8InExe();

        /*segment prefix check*/
        while(continue_check)
        {
            switch(opcode)
            {
            case(0x26)://es prefix
            {
                seg_reg_replace_ds = &seg_reg_es;
                seg_reg_replace_ss = &seg_reg_es;
                opcode = ReadData8InExe();
                break;
            }

            case(0x2E)://cs prefix
            {
                seg_reg_replace_ds = &seg_reg_cs;
                seg_reg_replace_ss = &seg_reg_cs;
                opcode = ReadData8InExe();
                break;
            }

            case(0x36)://ss prefix
            {
                seg_reg_replace_ds = &seg_reg_ss;
                seg_reg_replace_ss = &seg_reg_ss;
                opcode = ReadData8InExe();
                break;
            }

            case(0x3E)://ds prefix
            {
                seg_reg_replace_ds = &seg_reg_ds;
                seg_reg_replace_ss = &seg_reg_ds;
                opcode = ReadData8InExe();
                break;
            }
            /*REP prefix*/
            case(0xF3)://REP,REPE,REPZ
            {
                rep = 1;
                opcode = ReadData8InExe();
                break;
            }
            case(0xF2)://REPNE,REPNZ
            {
                rep = 2;
                opcode = ReadData8InExe();
                break;
            }
            default:
            {
                continue_check = 0;
                break;
            }
            }
        }

        count_code++;

        switch(opcode)
        {
        case(0x00)://ADD Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADDB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit), "r"(control_reg_flag)      /* input */
                :"eax"
            );
            break;
        }

        case(0x01)://ADD Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADDW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x02)://ADD Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADDB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x03)://ADD Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "addw %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x04)://ADD AL Ib
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADDB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*universal_reg_al), "=r"(control_reg_flag) /* output */
                :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x05)://ADD AX Iv
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADDW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x06)://PUSH ES
        {
            Push(seg_reg_es);
            break;
        }

        case(0x07)://POP ES
        {
            seg_reg_es = Pop();
            break;
        }

        case(0x08)://OR Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ORB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x09)://OR Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ORW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x0A)://OR Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ORB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x0B)://OR Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ORW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x0C)://OR AL Ib
        {
            opt1_8bit = universal_reg_al;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ORB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x0D)://OR AX Iv
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ORW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x0E)://PUSH CS
        {
            Push(seg_reg_cs);
            break;
        }

        case(0x0F)://POP CS
        {
            seg_reg_cs = Pop();
            break;
        }

        case(0x10)://ADC Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADCB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x11)://ADC Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADCW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x12)://ADC Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADCB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x13)://ADC Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADCW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x14)://ADC AL Ib
        {
            opt1_8bit = universal_reg_al;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADCB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x15)://ADC AX Iv
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ADCW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }
        case(0x16)://PUSH SS
        {
            Push(seg_reg_ss);
            break;
        }

        case(0x17)://POP SS
        {
            seg_reg_ss = Pop();
            break;
        }

        case(0x18)://SBB Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SUBB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x19)://SBB Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SBBW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x1A)://SBB Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SBBB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x1B)://SBB Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SBBW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x1C)://SBB AL Ib
        {
            opt1_8bit = universal_reg_al;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SBBB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x1D)://SBB AX Iv
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SBBW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x1E)://PUSH DS
        {
            Push(seg_reg_ds);
            break;
        }

        case(0x1F)://POP DS
        {
            seg_reg_ds = Pop();
            break;
        }

        case(0x20)://AND Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ANDB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x21)://AND Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ANDW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x22)://AND Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ANDB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x23)://AND Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ANDW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x24)://AND AL Ib
        {
            opt1_8bit = universal_reg_al;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ANDB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x25)://AND AX Iv
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "ANDW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        /*no case(0x26).0x26 is ES prefix*/

        case(0x27)://DAA AL
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVB %0,%%AL;\n\t"
                "DAA;\n\t"
                "MOVB %%AL,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*universal_reg_al), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /*input*/
                :"eax"
            );
            break;
        }

        case(0x28)://SUB Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SUBB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x29)://SUB Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SUBW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x2A)://SUB Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SUBB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x2B)://SUB Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SUBW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x2C)://SUB AL Ib
        {
            opt1_8bit = universal_reg_al;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SUBB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x2D)://SUB AX Iv
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SUBW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        /*no case(0x2E).0x2E is CS prefix*/

        case(0x2F)://DAS AL
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVB %0,%%AL;\n\t"
                "DAS;\n\t"
                "MOVB %%AL,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*universal_reg_al), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x30)://XOR Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "XORB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x31)://XOR Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "XORW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x32)://XOR Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "XORB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_8bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x33)://XOR Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "XORW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit)       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x34)://XOR AL Ib
        {
            opt1_8bit = universal_reg_al;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "XORB %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x35)://XOR AX Iv
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "XORW %2,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        /*no case(0x36).0x36 is SS prefix*/

        case(0x37)://AAA AL AH
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "MOVB %0,%%AL;\n\t"
                "MOVB %1,%%AH;\n\t"
                "AAA;\n\t"
                "MOVB %%AL,%0;\n\t"
                "MOVB %%AH,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%2;\n\t"
                :"+r"(*universal_reg_al), "+r"(*universal_reg_ah), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x38)://CMP Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPB %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_8bit), "r"(*opt2_8bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x39)://CMP Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPW %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_16bit), "r"(*opt2_16bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x3A)://CMP Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPB %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_8bit), "r"(*opt2_8bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x3B)://CMP Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            //*opt1_16bit = *opt1_16bit + *opt2_16bit;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPW %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_16bit), "r"(*opt2_16bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x3C)://CMP AL Ib
        {
            opt1_8bit = universal_reg_al;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPB %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_8bit), "r"(ReadData8InExe()), "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x3D)://CMP AX Iv
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "SUBW %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(universal_reg_ax), "r"(ReadData16InExe())      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        /*no case(0x3E).0x3E is DS prefix*/

        case(0x3F)://AAS AL AH
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "MOVB %0,%%AL;\n\t"
                "MOVB %1,%%AH;\n\t"
                "AAS;\n\t"
                "MOVB %%AL,%0;\n\t"
                "MOVB %%AH,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%2;\n\t"
                :"+r"(*universal_reg_al), "+r"(*universal_reg_ah), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x40)://INC eAX
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "INCW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }
        case(0x41)://INC eCX
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "INCW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_cx), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }
        case(0x42)://INC eDX
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "INCW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_dx), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }
        case(0x43)://INC eBX
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "INCW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_bx), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x44)://INC eSP
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "INCW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_sp), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /*input */
                :"eax"
            );
            break;
        }

        case(0x45)://INC eBP
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "INCW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_bp), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x46)://INC eSI
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "INCW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_si), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x47)://INC eDI
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "INCW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_di), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x48)://DEC eAX
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "DECW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }
        case(0x49)://DEC eCX
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "DECW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_cx), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }
        case(0x4A)://DEC eDX
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "DECW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_dx), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }
        case(0x4B)://DEC eBX
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "DECW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_bx), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x4C)://DEC eSP
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "DECW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_sp), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x4D)://DEC eBP
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "DECW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_bp), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x4E)://DEC eSI
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "DECW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_si), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x4F)://DEC eDI
        {
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %2;\n\t"
                "POPFW;\n\t"
                "MOVW %0,%%AX;\n\t"
                "DECW %%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"+r"(universal_reg_di), "=r"(control_reg_flag) /* output */
                :"r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }


        case(0x50)://PUSH AX
        {
            Push(universal_reg_ax);
            break;
        }
        case(0x51)://PUSH CX
        {
            Push(universal_reg_cx);
            break;
        }
        case(0x52)://PUSH DX
        {
            Push(universal_reg_dx);
            break;
        }
        case(0x53)://PUSH BX
        {
            Push(universal_reg_bx);
            break;
        }
        case(0x54)://PUSH SP
        {
            Push(universal_reg_sp);
            break;
        }
        case(0x55)://PUSH BP
        {
            Push(universal_reg_bp);
            break;
        }
        case(0x56)://PUSH SI
        {
            Push(universal_reg_si);
            break;
        }
        case(0x57)://PUSH DI
        {
            Push(universal_reg_di);
            break;
        }
        case(0x58)://POP AX
        {
            universal_reg_ax = Pop();
            break;
        }

        case(0x59)://POP CX
        {
            universal_reg_cx = Pop();
            break;
        }

        case(0x5A)://POP DX
        {
            universal_reg_dx = Pop();
            break;
        }

        case(0x5B)://POP BX
        {
            universal_reg_bx = Pop();
            break;
        }

        case(0x5C)://POP SP
        {
            universal_reg_sp = Pop();
            break;
        }

        case(0x5D)://POP BP
        {
            universal_reg_bp = Pop();
            break;
        }

        case(0x5E)://POP SI
        {
            universal_reg_si = Pop();
            break;
        }

        case(0x5F)://POP DI
        {
            universal_reg_di = Pop();
            break;
        }
#ifdef CPU_80186
        case(0x60)://PUSHA (80186)
        {
            uint16_t sp_tmp = universal_reg_sp;
            Push(universal_reg_ax);
            Push(universal_reg_cx);
            Push(universal_reg_dx);
            Push(universal_reg_bx);
            Push(sp_tmp);
            Push(universal_reg_bp);
            Push(universal_reg_si);
            Push(universal_reg_di);
            break;
        }

        case(0x61)://POPA (80186)
        {
            universal_reg_di = Pop();
            universal_reg_si = Pop();
            universal_reg_bp = Pop();
            Pop();
            universal_reg_bx = Pop();
            universal_reg_dx = Pop();
            universal_reg_cx = Pop();
            universal_reg_ax = Pop();
            break;
        }

        case(0x62)://BOUND Gv Ma (80186) remain to fix
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            if(static_cast<int32_t>(static_cast<int16_t>(*opt1_16bit)) < static_cast<int32_t>(static_cast<int16_t>(*opt2_16bit)))
                Intcall(5);
            else if(static_cast<int32_t>(static_cast<int16_t>(*opt1_16bit)) > static_cast<int32_t>(static_cast<int16_t>(*(opt2_16bit + 1))))
                Intcall(5);
            break;
        }

        //case(0x63)://ARPL Ew Gw (80286)
        //case(0x64)://FS prefix (80386)
        //case(0x65)://GS prefix (80386)
        //case(0x66)://Operand-size override prefix (80386)
        //case(0x67)://Address-size override prefix (80386)

        case(0x68)://PUSH Iz (80186)
        {
            Push(ReadData16InExe());
            break;
        }

        case(0x69)://IMUL Gv Ev Iz (80186)
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %4;\n\t"
                "POPFW;\n\t"
                "MOVW %3,%%AX;\n\t"
                "IMULW %2,%%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"=r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit), "r"(ReadData16InExe())   , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x6A)://PUSH Ib (80186)
        {
            Push(static_cast<uint16_t>(ReadData8InExe()));
            break;
        }

        case(0x6B)://IMUL Gv Ev Ib (80186)
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %4;\n\t"
                "POPFW;\n\t"
                "MOVW %3,%%AX;\n\t"
                "IMULW %2,%%AX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%1;\n\t"
                :"=r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                :"r"(*opt2_16bit), "r"(static_cast<uint16_t>(ReadData8InExe())), "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        /* remain to fix */

        case(0x6C)://INSB (80186)
        {
            if(rep && universal_reg_cx == 0)
                break;
            WriteRam8((seg_reg_es << 4) + universal_reg_si, ports_operate->port_handle_read8(universal_reg_dx));
            uint8_t flag_df = (control_reg_flag >> 10) & 0x1;
            if(flag_df)
            {
                universal_reg_si = universal_reg_si - 1;
                universal_reg_di = universal_reg_di - 1;
            }
            else
            {
                universal_reg_si = universal_reg_si + 1;
                universal_reg_di = universal_reg_di + 1;
            }

            if(rep)
            {
                universal_reg_cx = universal_reg_cx - 1;
            }
            else
            {
                break;
            }
            control_reg_ip = firstip;
            break;
        }



        /* remain to fix */

        case(0x6D)://INSD (80186)
        {
            if(rep && universal_reg_cx == 0)
                break;
            WriteRam16((seg_reg_es << 4) + universal_reg_si, ports_operate->port_handle_read16(universal_reg_dx));
            uint8_t flag_df = (control_reg_flag >> 10) & 0x1;
            if(flag_df) {
                universal_reg_si = universal_reg_si - 2;
                universal_reg_di = universal_reg_di - 2;
            }
            else {
                universal_reg_si = universal_reg_si + 2;
                universal_reg_di = universal_reg_di + 2;
            }

            if(rep) {
                universal_reg_cx = universal_reg_cx - 1;
            }

            if(!rep) {
                break;
            }

            control_reg_ip = firstip;
            break;
        }


        /* remain to fix */

        case(0x6E)://OUSB (80186)
        {
            if(rep && universal_reg_cx == 0)
                break;
            ports_operate->port_handle_write8(universal_reg_dx, ReadRam8((*seg_reg_replace_ds << 4) + universal_reg_si));
            uint8_t flag_df = (control_reg_flag >> 10) & 0x1;
            if(flag_df) {
                universal_reg_si = universal_reg_si - 1;
                universal_reg_di = universal_reg_di - 1;
            }
            else {
                universal_reg_si = universal_reg_si + 1;
                universal_reg_di = universal_reg_di + 1;
            }

            if(rep) {
                universal_reg_cx = universal_reg_cx - 1;
            }

            if(!rep) {
                break;
            }

            control_reg_ip = firstip;
            break;
        }


        /* remain to fix */

        case(0x6F)://OUSW (80186)
        {
            if(rep && universal_reg_cx == 0)
                break;
            ports_operate->port_handle_write16(universal_reg_dx, ReadRam16((*seg_reg_replace_ds << 4) + universal_reg_si));
            uint8_t flag_df = (control_reg_flag >> 10) & 0x1;
            if(flag_df) {
                universal_reg_si = universal_reg_si - 2;
                universal_reg_di = universal_reg_di - 2;
            }
            else {
                universal_reg_si = universal_reg_si + 2;
                universal_reg_di = universal_reg_di + 2;
            }

            if(rep) {
                universal_reg_cx = universal_reg_cx - 1;
            }

            if(!rep) {
                break;
            }

            control_reg_ip = firstip;
            break;
        }
#endif
        case(0x70)://JO Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_of = (control_reg_flag >> 11) & 0x1;
            if(flag_of)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x71)://JNO Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_of = (control_reg_flag >> 11) & 0x1;
            if(!flag_of)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x72)://JB Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_cf = control_reg_flag & 0x1;
            if(flag_cf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x73)://JNB Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_cf = control_reg_flag & 0x1;
            if(!flag_cf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x74)://JZ Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_zf = (control_reg_flag >> 6) & 0x1;
            if(flag_zf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x75)://JNZ Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_zf = (control_reg_flag >> 6) & 0x1;
            if(flag_zf == 0)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x76)://JBE Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_cf = control_reg_flag & 0x1;
            uint8_t flag_zf = (control_reg_flag >> 6) & 0x1;
            if(flag_zf || flag_cf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x77)://JA Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_cf = control_reg_flag & 0x1;
            uint8_t flag_zf = (control_reg_flag >> 6) & 0x1;
            if(flag_zf == 0 && flag_cf == 0)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x78)://JS Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_sf = (control_reg_flag >> 7) & 0x1;
            if(flag_sf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x79)://JNS Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_sf = (control_reg_flag >> 7) & 0x1;
            if(!flag_sf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x7A)://JPE Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_pf = (control_reg_flag >> 2) & 0x1;
            if(flag_pf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x7B)://JPO Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_pf = (control_reg_flag >> 2) & 0x1;
            if(!flag_pf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x7C)://JL Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_sf = (control_reg_flag >> 7) & 0x1;
            uint8_t flag_of = (control_reg_flag >> 11) & 0x1;
            if(flag_sf != flag_of)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x7D)://JNL Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_sf = (control_reg_flag >> 7) & 0x1;
            uint8_t flag_of = (control_reg_flag >> 11) & 0x1;
            if(flag_sf == flag_of)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x7E)://JLE Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_zf = (control_reg_flag >> 6) & 0x1;
            uint8_t flag_sf = (control_reg_flag >> 7) & 0x1;
            uint8_t flag_of = (control_reg_flag >> 11) & 0x1;
            if(flag_zf || flag_of != flag_sf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x7F)://JNLE Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            uint8_t flag_zf = (control_reg_flag >> 6) & 0x1;
            uint8_t flag_sf = (control_reg_flag >> 7) & 0x1;
            uint8_t flag_of = (control_reg_flag >> 11) & 0x1;
            if(flag_zf == 0 && flag_of == flag_sf)
                control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0x80)://group Eb Ib
        case(0x82)://group Eb Ib (i64)
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_8bit = CalculateRM(mod_byte, opcode);
            switch(group_index)
            {
            case(0x00)://ADD Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ADDB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://OR Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ORB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://ADC Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ADCB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://SBB Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SBBB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://AND Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ANDB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SUB Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SUBB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://XOR Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "XORB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://CMP Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "CMPB %2,%1;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    :"=r"(control_reg_flag) /* output */
                    :"r"(*opt1_8bit), "r"(ReadData8InExe())      , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0x81)://group Ev Iz
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            switch(group_index)
            {
            case(0x00)://ADD Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ADDW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://OR Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ORW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://ADC Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ADCW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://SBB Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SBBW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://AND Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ANDW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SUB Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SUBW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://XOR Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "XORW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://CMP Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "CMPW %2,%1;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    :"=r"(control_reg_flag) /* output */
                    :"r"(*opt1_16bit), "r"(ReadData16InExe())      , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0x83)://group Ev Ib
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            switch(group_index)
            {
            case(0x00)://ADD Ev Ib
            {
                int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>((ReadData8InExe())));
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ADDW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(tmp_data)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://OR Ev Ib
            {
                int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>((ReadData8InExe())));
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ORW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(tmp_data)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://ADC Ev Ib
            {
                int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>((ReadData8InExe())));
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ADCW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(tmp_data)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://SBB Ev Ib
            {
                int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>((ReadData8InExe())));
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SBBW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(tmp_data)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://AND Ev Ib
            {
                int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>((ReadData8InExe())));
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ANDW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(tmp_data)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SUB Ev Ib
            {
                int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>((ReadData8InExe())));
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SUBW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(tmp_data)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://XOR Ev Ib
            {
                int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>((ReadData8InExe())));
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "XORW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(tmp_data)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://CMP Ev Ib
            {
                int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>((ReadData8InExe())));
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "CMPW %2,%1;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    :"=r"(control_reg_flag) /* output */
                    :"r"(*opt1_16bit), "r"(tmp_data)      , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0x84)://TEST Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "TESTB %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_8bit), "r"(*opt2_8bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x85)://TEST Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "TESTW %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_16bit), "r"(*opt2_16bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0x86)://XCHG Eb Gb
        {
            uint8_t tmp_data;
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            tmp_data = *opt1_8bit;
            *opt1_8bit = *opt2_8bit;
            *opt2_8bit = tmp_data;
            break;
        }

        case(0x87)://XCHG Ev Gv
        {
            uint16_t tmp_data;
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            tmp_data = *opt1_16bit;
            *opt1_16bit = *opt2_16bit;
            *opt2_16bit = tmp_data;
            break;
        }

        case(0x88)://MOV Eb Gb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            *opt1_8bit = *opt2_8bit;
            break;
        }

        case(0x89)://MOV Ev Gv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateReg16(mod_byte);
            *opt1_16bit = *opt2_16bit;
            break;
        }

        case(0x8A)://MOV Gb Eb
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateReg8(mod_byte);
            opt2_8bit = CalculateRM(mod_byte, opcode);
            *opt1_8bit = *opt2_8bit;
            break;
        }

        case(0x8B)://MOV Gv Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            *opt1_16bit = *opt2_16bit;
            break;
        }

        case(0x8C)://MOV Rw Sw
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            opt2_16bit = CalculateSeg16(mod_byte);
            *opt1_16bit = *opt2_16bit;
            break;
        }

        case(0x8D)://LEA Gv M
        {
            mod_byte = ReadData8InExe();
            uint8_t rm = mod_byte & 0x7;
            uint8_t mod_bit = (mod_byte >> 6) & 0x3;
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            if((mod_bit == 0x0 && (rm == 0x2 || rm == 0x3)) || (mod_bit != 0x0 && (rm == 0x2 || rm == 0x3 || rm == 0x6)))
                *opt1_16bit = (reinterpret_cast<uint8_t  *>(opt2_16bit) - &ram[0]) - (*seg_reg_replace_ss << 4);
            else
                *opt1_16bit = (reinterpret_cast<uint8_t  *>(opt2_16bit) - &ram[0]) - (*seg_reg_replace_ds << 4);
            break;
        }

        case(0x8E)://MOV Sw Rw
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateSeg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode | 0x1));
            //  if(old == 0x33)
            //      printf("%x<-%x\n",*opt1_16bit,*opt2_16bit);
            *opt1_16bit = *opt2_16bit;
            break;
        }

        case(0x8F)://POP Ev
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            *opt1_16bit = Pop();
            break;
        }

        case(0x90)://NOP
        {
            break;
        }

        case(0x91)://XCHG eCX eAX
        {
            uint16_t tmp_data;
            tmp_data = universal_reg_ax;
            universal_reg_ax = universal_reg_cx;
            universal_reg_cx = tmp_data;
            break;
        }

        case(0x92)://XCHG eDX eAX
        {
            uint16_t tmp_data;
            tmp_data = universal_reg_ax;
            universal_reg_ax = universal_reg_dx;
            universal_reg_dx = tmp_data;
            break;
        }

        case(0x93)://XCHG eBX eAX
        {
            uint16_t tmp_data;
            tmp_data = universal_reg_ax;
            universal_reg_ax = universal_reg_bx;
            universal_reg_bx = tmp_data;
            break;
        }

        case(0x94)://XCHG eSP eAX
        {
            uint16_t tmp_data;
            tmp_data = universal_reg_ax;
            universal_reg_ax = universal_reg_sp;
            universal_reg_sp = tmp_data;
            break;
        }

        case(0x95)://XCHG eBP eAX
        {
            uint16_t tmp_data;
            tmp_data = universal_reg_ax;
            universal_reg_ax = universal_reg_bp;
            universal_reg_bp = tmp_data;
            break;
        }

        case(0x96)://XCHG eSI eAX
        {
            uint16_t tmp_data;
            tmp_data = universal_reg_ax;
            universal_reg_ax = universal_reg_si;
            universal_reg_si = tmp_data;
            break;
        }

        case(0x97)://XCHG eDI eAX
        {
            uint16_t tmp_data;
            tmp_data = universal_reg_ax;
            universal_reg_ax = universal_reg_di;
            universal_reg_di = tmp_data;
            break;
        }

        case(0x98)://CBW
        {
            if((*universal_reg_al & 0x80) == 0x80)
                *universal_reg_ah = 0xFF;
            else
                *universal_reg_ah = 0x0;
            break;
        }

        case(0x99)://CWD
        {
            if((*universal_reg_ah & 0x80) == 0x80)
                universal_reg_dx = 0xFFFF;
            else
                universal_reg_dx = 0x0;
            break;
        }

        case(0x9A)://CALL Ap
        {
            uint16_t offset = ReadData16InExe();//offset
            uint16_t seletor = ReadData16InExe();//seletor
            Push(seg_reg_cs);
            Push(control_reg_ip);
            control_reg_ip = offset;
            seg_reg_cs = seletor;
            break;
        }

        case(0x9B)://WAIT
        {
            break;
        }

        case(0x9C)://PUSHF
        {
            if(if_flag == 1)
                if(tf_flag == 1)
                    Push(control_reg_flag | 0x0300);
                else
                    Push((control_reg_flag | 0x0200) & 0xFEFF);
            else if(tf_flag == 0)
                Push(control_reg_flag & 0xFCFF);
            else
                Push((control_reg_flag | 0x0100) & 0xFDFF);
            break;
        }

        case(0x9D)://POPF
        {
            control_reg_flag = Pop();
            if_flag = (control_reg_flag >> 9) & 0x1;
            tf_flag = (control_reg_flag >> 8) & 0x1;
            break;
        }

        case(0x9E)://SAHF
        {
            control_reg_flag = (control_reg_flag & 0xFF00) | ((*universal_reg_ah & 0xD7) | 0x2);
            break;
        }

        case(0x9F)://LAHF
        {
            *universal_reg_ah = ((control_reg_flag & 0xD7) | 0x2);
            break;
        }

        case(0xA0)://MOV AL Ob
        {
            uint16_t offset = ReadData16InExe();//offset
            *universal_reg_al = ReadRam8((*seg_reg_replace_ds << 4) + offset);
            break;
        }

        case(0xA1)://MOV eAX Ov
        {
            uint16_t offset = ReadData16InExe();//offset
            universal_reg_ax = ReadRam16((*seg_reg_replace_ds << 4) + offset);
            break;
        }

        case(0xA2)://MOV Ob AL
        {
            uint16_t offset = ReadData16InExe();//offset
            WriteRam8((*seg_reg_replace_ds << 4) + offset, *universal_reg_al);
            break;
        }

        case(0xA3)://MOV Ov eAX
        {
            uint16_t offset = ReadData16InExe();//offset
            WriteRam16((*seg_reg_replace_ds << 4) + offset, universal_reg_ax);
            break;
        }

        /*remain to fix*/

        case(0xA4)://MOVS Yb Xb
        {
            if(rep && (universal_reg_cx == 0))
                break;

            uint8_t df = control_reg_flag >> 10 & 0x1;
            WriteRam8((seg_reg_es << 4) + universal_reg_di, ReadRam8((*seg_reg_replace_ds << 4) + universal_reg_si));

            if(df)
            {
                universal_reg_di = universal_reg_di - 1;
                universal_reg_si = universal_reg_si - 1;
            }
            else
            {
                universal_reg_di = universal_reg_di + 1;
                universal_reg_si = universal_reg_si + 1;
            }

            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            else
                break;

            control_reg_ip = firstip;
            break;
        }

        /*remain to fix*/
        case(0xA5)://MOVS Yv Xv
        {
            if(rep && (universal_reg_cx == 0))
                break;

            uint8_t df = control_reg_flag >> 10 & 0x1;
            WriteRam16((seg_reg_es << 4) + universal_reg_di, ReadRam16((*seg_reg_replace_ds << 4) + universal_reg_si));

            if(df)
            {
                universal_reg_di = universal_reg_di - 2;
                universal_reg_si = universal_reg_si - 2;
            }
            else
            {
                universal_reg_di = universal_reg_di + 2;
                universal_reg_si = universal_reg_si + 2;
            }

            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            else
                break;

            control_reg_ip = firstip;
            break;
        }

        /*remain to fix*/
        case(0xA6)://CMPS Yb Xb
        {

            if(rep && (universal_reg_cx == 0))
                break;

            uint8_t opt1_8bit = ReadRam8((*seg_reg_replace_ds << 4) + universal_reg_si);
            uint8_t opt2_8bit = ReadRam8((seg_reg_es << 4) + universal_reg_di);

            uint8_t df = control_reg_flag >> 10 & 0x1;
            if(df)
            {
                universal_reg_di = universal_reg_di - 1;
                universal_reg_si = universal_reg_si - 1;
            }
            else
            {
                universal_reg_di = universal_reg_di + 1;
                universal_reg_si = universal_reg_si + 1;
            }

            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPB %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(opt1_8bit), "r"(opt2_8bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );

            uint8_t zf = (control_reg_flag >> 6) & 0x1;
            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            if(rep == 1 && zf == 0)
                break;
            else if(rep == 2 && zf == 1)
                break;
            if(rep == 0)
                break;
            control_reg_ip = firstip;
            break;
        }

        /*remain to fix*/
        case(0xA7)://CMPS Yv Xv
        {
            if(rep && (universal_reg_cx == 0))
                break;

            uint16_t opt1_16bit = ReadRam16((*seg_reg_replace_ds << 4) + universal_reg_si);
            uint16_t opt2_16bit = ReadRam16((seg_reg_es << 4) + universal_reg_di);

            uint8_t df = control_reg_flag >> 10 & 0x1;
            if(df)
            {
                universal_reg_di = universal_reg_di - 2;
                universal_reg_si = universal_reg_si - 2;
            }
            else
            {
                universal_reg_di = universal_reg_di + 2;
                universal_reg_si = universal_reg_si + 2;
            }

            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPW %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(opt1_16bit), "r"(opt2_16bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );

            uint8_t zf = (control_reg_flag >> 6) & 0x1;
            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            if(rep == 1 && zf == 0)
                break;
            else if(rep == 2 && zf == 1)
                break;
            if(rep == 0)
                break;
            control_reg_ip = firstip;
            break;
        }

        case(0xA8)://TEST AL Ib
        {
            opt1_8bit = universal_reg_al;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "TESTB %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_8bit), "r"(ReadData8InExe()), "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0xA9)://TEST eAX Iz
        {
            opt1_16bit = &universal_reg_ax;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "TESTW %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_16bit), "r"(ReadData16InExe())      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        /*remain to fix*/
        case(0xAA)://STOS Yb AL
        {
            if(rep && (universal_reg_cx == 0))
                break;

            uint8_t df = control_reg_flag >> 10 & 0x1;
            WriteRam8((seg_reg_es << 4) + universal_reg_di, *universal_reg_al);

            if(df)
            {
                universal_reg_di = universal_reg_di - 1;
            }
            else
            {
                universal_reg_di = universal_reg_di + 1;
            }

            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            else
                break;

            control_reg_ip = firstip;
            break;
        }

        /*remain to fix*/
        case(0xAB)://STOS Yv rAX
        {
            if(rep && (universal_reg_cx == 0))
                break;
            uint8_t df = control_reg_flag >> 10 & 0x1;
            WriteRam16((seg_reg_es << 4) + universal_reg_di, universal_reg_ax);
            if(df)
            {
                universal_reg_di = universal_reg_di - 2;
            }
            else
            {
                universal_reg_di = universal_reg_di + 2;
            }

            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            else
                break;

            control_reg_ip = firstip;
            break;
        }

        /*remain to fix*/
        case(0xAC)://LODS AL Xb
        {
            if(rep && (universal_reg_cx == 0))
                break;

            uint8_t df = control_reg_flag >> 10 & 0x1;
            *universal_reg_al = ReadRam8((*seg_reg_replace_ds << 4) + universal_reg_si);

            if(df)
            {
                universal_reg_si = universal_reg_si - 1;
            }
            else
            {
                universal_reg_si = universal_reg_si + 1;
            }

            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            else
                break;

            control_reg_ip = firstip;
            break;
        }

        /*remain to fix*/
        case(0xAD)://LODS rAX Xv
        {
            if(rep && (universal_reg_cx == 0))
                break;

            uint8_t df = control_reg_flag >> 10 & 0x1;
            universal_reg_ax = ReadRam16((*seg_reg_replace_ds << 4) + universal_reg_di);

            if(df)
            {
                universal_reg_si = universal_reg_si - 2;
            }
            else
            {
                universal_reg_si = universal_reg_si + 2;
            }

            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            else
                break;

            control_reg_ip = firstip;
            break;
        }

        /*remain to fix*/
        case(0xAE)://SCAS Yb AL
        {
            if(rep && (universal_reg_cx == 0))
                break;
            opt1_8bit = universal_reg_al;
            uint8_t opt2_8bit = ReadRam8((seg_reg_es << 4) + universal_reg_di);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPB %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*opt1_8bit), "r"(opt2_8bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            uint8_t df = control_reg_flag >> 10 & 0x1;
            if(df)
            {
                universal_reg_di = universal_reg_di - 1;
            }
            else
            {
                universal_reg_di = universal_reg_di + 1;
            }

            uint8_t zf = (control_reg_flag >> 6) & 0x1;
            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            if(rep == 1 && zf == 0)
                break;
            else if(rep == 2 && zf == 1)
                break;
            if(rep == 0)
                break;
            control_reg_ip = firstip;
            break;
        }

        /*remain to fix*/
        case(0xAF)://SCAS Yv rAX
        {
            if(rep && (universal_reg_cx == 0))
                break;

            uint16_t opt1_16bit = universal_reg_ax;
            uint16_t opt2_16bit = ReadRam16((seg_reg_es << 4) + universal_reg_di);
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "CMPW %2,%1;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(opt1_16bit), "r"(opt2_16bit)      , "r"(control_reg_flag) /* input */
                :"eax"
            );
            uint8_t df = control_reg_flag >> 10 & 0x1;
            if(df)
            {
                universal_reg_di = universal_reg_di - 2;
            }
            else
            {
                universal_reg_di = universal_reg_di + 2;
            }


            uint8_t zf = (control_reg_flag >> 6) & 0x1;
            if(rep)
                universal_reg_cx = universal_reg_cx - 1;
            if(rep == 1 && zf == 0)
                break;
            else if(rep == 2 && zf == 1)
                break;
            if(rep == 0)
                break;
            control_reg_ip = firstip;
            break;
        }

        case(0xB0)://MOV AL Ib
        {
            *universal_reg_al = ReadData8InExe();
            break;
        }

        case(0xB1)://MOV CL Ib
        {
            *universal_reg_cl = ReadData8InExe();
            break;
        }

        case(0xB2)://MOV DL Ib
        {
            *universal_reg_dl = ReadData8InExe();
            break;
        }

        case(0xB3)://MOV BL Ib
        {
            *universal_reg_bl = ReadData8InExe();
            break;
        }

        case(0xB4)://MOV AH Ib
        {
            *universal_reg_ah = ReadData8InExe();
            break;
        }

        case(0xB5)://MOV CH Ib
        {
            *universal_reg_ch = ReadData8InExe();
            break;
        }

        case(0xB6)://MOV DH Ib
        {
            *universal_reg_dh = ReadData8InExe();
            break;
        }

        case(0xB7)://MOV BH Ib
        {
            *universal_reg_bh = ReadData8InExe();
            break;
        }

        case(0xB8)://MOV eAX Iv
        {
            universal_reg_ax = ReadData16InExe();
            break;
        }

        case(0xB9)://MOV eCX Iv
        {
            universal_reg_cx = ReadData16InExe();
            break;
        }

        case(0xBA)://MOV eDX Iv
        {
            universal_reg_dx = ReadData16InExe();
            break;
        }

        case(0xBB)://MOV eBX Iv
        {
            universal_reg_bx = ReadData16InExe();
            break;
        }

        case(0xBC)://MOV SP Iv
        {
            universal_reg_sp = ReadData16InExe();
            break;
        }

        case(0xBD)://MOV BP Iv
        {
            universal_reg_bp = ReadData16InExe();
            break;
        }
        case(0xBE)://MOV SI Iv
        {
            universal_reg_si = ReadData16InExe();
            break;
        }
        case(0xBF)://MOV DI Iv
        {
            universal_reg_di = ReadData16InExe();
            break;
        }

        case(0xC0)://group Eb Ib (80186)
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_8bit = CalculateRM(mod_byte, opcode);
            switch(group_index)
            {
            case(0x00)://ROL Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ROLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://ROR Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RORB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://RCL Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe()), "r"(control_reg_flag)  /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://RCR Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCRB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe()), "r"(control_reg_flag)  /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://SHL Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SHR Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHRB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://SAL Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SALB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://SAR Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SARB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0xC1)://group Ev Ib
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            switch(group_index)
            {
            case(0x00)://ROL Ev Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ROLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://ROR Ev Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RORW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://RCL Ev Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe()), "r"(control_reg_flag)     /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://RCR Ev Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCRW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe()), "r"(control_reg_flag)      /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://SHL Ev Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SHR Ev Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHRW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://SAL Ev Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SALW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://SAR Ev Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SARW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0xC2)://RETN Iw
        {
            control_reg_ip = Pop();
            universal_reg_sp = universal_reg_sp + ReadData16InExe();
            break;
        }

        case(0xC3)://RETN
        {

            control_reg_ip = Pop();
            break;
        }

        case(0xC4)://LES Gv Mp
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            *opt1_16bit = *opt2_16bit;
            seg_reg_es = *(opt2_16bit + 1);
            break;
        }

        case(0xC5)://LDS Gv Mp
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = CalculateReg16(mod_byte);
            opt2_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            *opt1_16bit = *opt2_16bit;
            seg_reg_ds = *(opt2_16bit + 1);
            break;
        }

        case(0xC6)://MOV Eb Ib
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            *opt1_8bit = ReadData8InExe();
            break;
        }

        case(0xC7)://MOV Ev Iv
        {
            mod_byte = ReadData8InExe();
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            *opt1_16bit = ReadData16InExe();
            break;
        }

        case(0xC8)://ENTER Iw Ib(80186)
        {
            uint16_t stack_size = ReadData16InExe();
            uint8_t lexical_nesting_level = ReadData8InExe();
            Push(universal_reg_bp);
            uint16_t tmp_sp = universal_reg_sp;
            int for_count;
            if(lexical_nesting_level)
            {
                for(for_count = 1; for_count < lexical_nesting_level; for_count++)
                {
                    universal_reg_bp = universal_reg_bp - 2;
                    Push(universal_reg_bp);
                }
                Push(universal_reg_sp);
            }
            universal_reg_bp = tmp_sp;
            universal_reg_sp = universal_reg_bp - stack_size;
            break;
        }

        case(0xC9)://LEAVE
        {
            universal_reg_sp = universal_reg_bp;
            universal_reg_bp = Pop();
            break;
        }

        case(0xCA)://RETF Iw
        {
            control_reg_ip = Pop();
            seg_reg_cs = Pop();
            universal_reg_sp = universal_reg_sp + ReadData16InExe();
            break;
        }

        case(0xCB)://RETF
        {
            control_reg_ip = Pop();
            seg_reg_cs = Pop();
            break;
        }

        /*remain to fix*/
        case(0xCC)://INT 3
        {
            Intcall(3);
            break;
        }

        /*remain to fix*/
        case(0xCD)://INT Ib
        {
            Intcall(ReadData8InExe());
            break;
        }

        case(0xCE)://INTO
        {
            uint8_t of = (control_reg_flag >> 11) & 0x1;
            if(of)
            {
                Intcall(4);
            }
            break;
        }

        case(0xCF)://IRET
        {
            control_reg_ip = Pop();
            seg_reg_cs = Pop();
            control_reg_flag = Pop();
            if_flag = (control_reg_flag >> 9) & 0x1;
            tf_flag = (control_reg_flag >> 8) & 0x1;
            break;
        }

        case(0xD0)://group Eb 1
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_8bit = CalculateRM(mod_byte, opcode);
            switch(group_index)
            {
            case(0x00)://ROL Eb 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ROLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://ROR Eb 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RORB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://RCL Eb 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://RCR Eb 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCRB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://SHL Eb 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SHR Eb 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHRB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://SAL Eb 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SALB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://SAR Eb i
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SARB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0xD1)://group Ev 1
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            switch(group_index)
            {
            case(0x00)://ROL Ev 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ROLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://ROR Ev 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RORW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://RCL Ev 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://RCR Ev 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCRW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"i"(1) , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://SHL Ev 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SHR Ev 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHRW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://SAL Ev 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SALW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://SAR Ev 1
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SARW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"i"(1)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0xD2)://group Eb CL
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_8bit = CalculateRM(mod_byte, opcode);
            switch(group_index)
            {
            case(0x00)://ROL Eb CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ROLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://ROR Eb CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RORB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://RCL Eb CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl), "r"(control_reg_flag)   /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://RCR Eb CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCRB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl), "r"(control_reg_flag)     /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://SHL Eb CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHLB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SHR Eb CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHRB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://SAL Eb CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SALB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://SAR Eb CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SARB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0xD3)://group Ev CL
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            switch(group_index)
            {
            case(0x00)://ROL Ev CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "ROLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://ROR Ev CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RORW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://RCL Ev CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl), "r"(control_reg_flag)      /* input */
                    :"eax"
                );
                break;
            }

            case(0x03)://RCR Ev CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "RCRW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl), "r"(control_reg_flag)     /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://SHL Ev CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHLW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://SHR Ev CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SHRW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://SAL Ev CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SALW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://SAR Ev CL
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "SARW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(*universal_reg_cl)       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0xD4)://AAM Ib
        {
            uint8_t divisor = ReadData8InExe();
            if(!divisor)
            {
                Intcall(0);//divid by 0
                break;
            }
            *universal_reg_ah = *universal_reg_al / divisor;
            *universal_reg_al = *universal_reg_al % divisor;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHF;\n\t"
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "MOVW %1,%%AX;\n\t"
                "TESTW %2,%%AX;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "POPF;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(universal_reg_ax), "i"(0xFFFF) , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0xD5)://AAD Ib
        {
            uint8_t multiplier = ReadData8InExe();
            *universal_reg_al = (*universal_reg_al + (*universal_reg_ah * multiplier)) & 0xFF;
            *universal_reg_ah = 0x0;
            control_reg_flag &= 0xFEFF;
            __asm__
            (
                "PUSHF;\n\t"
                "PUSHW %3;\n\t"
                "POPFW;\n\t"
                "MOVB %1,%%AL;\n\t"
                "TESTB %2,%%AL;\n\t"
                "PUSHF;\n\t"
                "POP %%EAX;\n\t"
                "MOVW %%AX,%0;\n\t"
                "POPF;\n\t"
                :"=r"(control_reg_flag) /* output */
                :"r"(*universal_reg_al), "i"(0xFF) , "r"(control_reg_flag) /* input */
                :"eax"
            );
            break;
        }

        case(0xD6)://SALC
        {
            uint8_t cf = control_reg_flag & 0x1;
            if(cf)
                *universal_reg_al = 0xFF;
            else
                *universal_reg_al = 0x00;
            break;
        }

        case(0xD7)://XLATB
        {
            *universal_reg_al = ReadRam8(*seg_reg_replace_ds * 16 + universal_reg_bx + *universal_reg_al);
            break;
        }

        /*X87 opcode unsupport*/
        case(0xD8):
        case(0xD9):
        case(0xDA):
        case(0xDB):
        case(0xDC):
        case(0xDD):
        case(0xDE):
        case(0xDF):
        {
            mod_byte = ReadData8InExe();
            opt1_8bit = CalculateRM(mod_byte, opcode);
            opt2_8bit = CalculateReg8(mod_byte);
            control_reg_flag &= 0xFEFF;
            break;
        }

        case(0xE0)://LOOPNZ rel8 IP have error remain fix
        {
            uint8_t zf = (control_reg_flag >> 6) & 0x1;
            universal_reg_cx = universal_reg_cx - 1;
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            if(universal_reg_cx && !zf)
            {
                control_reg_ip = control_reg_ip + tmp_data;
            }
            break;
        }

        case(0xE1)://LOOPZ rel8 IP have error remain fix
        {
            uint8_t zf = (control_reg_flag >> 6) & 0x1;
            universal_reg_cx = universal_reg_cx - 1;
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            if(universal_reg_cx && zf)
            {
                control_reg_ip = control_reg_ip + tmp_data;
            }
            break;
        }

        case(0xE2)://LOOP rel8 IP have error remain fix
        {
            universal_reg_cx = universal_reg_cx - 1;
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            if(universal_reg_cx)
            {
                control_reg_ip = control_reg_ip + tmp_data;
            }
            break;
        }

        case(0xE3)://JCXZ rel8
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            if(!universal_reg_cx)
            {
                control_reg_ip = control_reg_ip + tmp_data;
            }
            break;
        }

        /*port IO operate remain to fix*/
        case(0xE4)://IN AL ib
        {
            uint8_t tmp_data = ReadData8InExe();
            *universal_reg_al = read8_from_port(tmp_data);
            break;
        }

        case(0xE5)://IN AX ib
        {
            uint8_t tmp_data = ReadData8InExe();
            universal_reg_ax = read16_from_port(tmp_data);
            break;
        }

        case(0xE6)://OUT Ib AL
        {
            uint8_t tmp_data = ReadData8InExe();
            write8_to_port(tmp_data, *universal_reg_al);
            break;
        }

        case(0xE7)://OUT Ib AX
        {
            uint8_t tmp_data = ReadData8InExe();
            write16_to_port(tmp_data, universal_reg_ax);
            break;
        }

        case(0xE8)://CALL Jv
        {
            uint16_t tmp_data = ReadData16InExe();
            Push(control_reg_ip);
            control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0xE9)://JMP Jv
        {
            uint16_t tmp_data = ReadData16InExe();
            control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        case(0xEA)://JMP Ap
        {
            uint16_t tmp_ip =  ReadData16InExe();
            uint16_t tmp_cs =  ReadData16InExe();
            control_reg_ip = tmp_ip;
            seg_reg_cs = tmp_cs;
            break;
        }

        case(0xEB)://JMP Jb
        {
            int16_t tmp_data = static_cast<int16_t>(static_cast<int8_t>(ReadData8InExe()));
            control_reg_ip = control_reg_ip + tmp_data;
            break;
        }

        /*port IO oprate remain to fix*/
        case(0xEC)://IN AL DX
        {
            *universal_reg_al = ports_operate->port_handle_read8(universal_reg_dx);
            break;
        }

        case(0xED)://IN AX DX
        {
            universal_reg_ax = ports_operate->port_handle_read16(universal_reg_dx);
            break;
        }

        case(0xEE)://OUT DX AL
        {
            ports_operate->port_handle_write8(universal_reg_dx, *universal_reg_al);
            break;
        }

        case(0xEF)://OUT DX AX
        {
            ports_operate->port_handle_write16(universal_reg_dx, universal_reg_ax);
            break;
        }

        case(0xF0)://LOCK
            break;

        case(0xF4)://HLT
        {
            halt = 1;
            break;
        }

        case(0xF5)://CMC
        {
            if(control_reg_flag & 0x1)
                control_reg_flag = control_reg_flag & 0xFFFE;
            else
                control_reg_flag = control_reg_flag | 0x1;
            break;
        }

        case(0xF6)://group Eb Ib
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_8bit = CalculateRM(mod_byte, opcode);
            switch(group_index)
            {
            case(0x00)://TEST Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "TESTB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://TEST Eb Ib
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "TESTB %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData8InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://NOT Eb
            {
                *opt1_8bit = ~(*opt1_8bit);
                break;
            }

            case(0x03)://NEG Eb
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %2;\n\t"
                    "POPFW;\n\t"
                    "NEGB %0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    : "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://MUL Eb
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "MOVW %0,%%AX;\n\t"
                    "MULB %2;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                    :"r"(*opt1_8bit)     , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x05)://IMUL Eb
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "MOVW %0,%%AX;\n\t"
                    "IMULB %2;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                    :"r"(*opt1_8bit)     , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x06)://DIV Eb
            {
                if(!*opt1_8bit)
                {
                    Intcall(0);//divid by 0
                    break;
                }
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "MOVW %0,%%AX;\n\t"
                    "DIVB %2;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                    :"r"(*opt1_8bit)     , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x07)://IDIV Eb
            {
                if(!*opt1_8bit)
                {
                    Intcall(0);//divid by 0
                    break;
                }
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "MOVW %0,%%AX;\n\t"
                    "IDIVB %2;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(universal_reg_ax), "=r"(control_reg_flag) /* output */
                    :"r"(*opt1_8bit)     , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }
            }
            break;
        }

        case(0xF7)://group Ev Iz
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            switch(group_index)
            {
            case(0x00)://TEST Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "TESTW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://TEST Ev Iz
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %3;\n\t"
                    "POPFW;\n\t"
                    "TESTW %2,%0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :"r"(ReadData16InExe())       , "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://NOT Ev
            {
                *opt1_16bit = ~(*opt1_16bit);
                break;
            }

            case(0x03)://NEG Ev
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %2;\n\t"
                    "POPFW;\n\t"
                    "NEGW %0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :    "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x04)://MUL Ev
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %4;\n\t"
                    "POPFW;\n\t"
                    "MOVW %0,%%AX;\n\t"
                    "MULW %3;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    "MOVW %%DX,%2;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(universal_reg_ax), "=r"(control_reg_flag), "=r"(universal_reg_dx) /* output */
                    :"r"(*opt1_16bit)     , "r"(control_reg_flag) /* input */
                    :"eax", "edx"
                );
                break;
            }

            case(0x05)://IMUL Ev
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %4;\n\t"
                    "POPFW;\n\t"
                    "MOVW %0,%%AX;\n\t"
                    "IMULW %3;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    "MOVW %%DX,%2;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(universal_reg_ax), "=r"(control_reg_flag), "=r"(universal_reg_dx) /* output */
                    :"r"(*opt1_16bit)     , "r"(control_reg_flag) /* input */
                    :"eax", "edx"
                );
                break;
            }

            case(0x06)://DIV Ev
            {
                if(!*opt1_16bit)
                {
                    Intcall(0);//divid by 0
                    break;
                }
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %4;\n\t"
                    "POPFW;\n\t"
                    "MOVW %0,%%AX;\n\t"
                    "DIVW %3;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    "MOVW %%DX,%2;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(universal_reg_ax), "=r"(control_reg_flag), "=r"(universal_reg_dx) /* output */
                    :"r"(*opt1_16bit)     , "r"(control_reg_flag) /* input */
                    :"eax", "edx"
                );
                break;
                break;
            }

            case(0x07)://IDIV Ev
            {
                if(!*opt1_16bit)
                {
                    Intcall(0);//divid by 0
                    break;
                }
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %4;\n\t"
                    "POPFW;\n\t"
                    "MOVW %0,%%AX;\n\t"
                    "IDIVW %3;\n\t"
                    "MOVW %%AX,%0;\n\t"
                    "MOVW %%DX,%2;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(universal_reg_ax), "=r"(control_reg_flag), "=r"(universal_reg_dx) /* output */
                    :"r"(*opt1_16bit)     , "r"(control_reg_flag) /* input */
                    :"eax", "edx"
                );
                break;
            }
            }
            break;
        }

        case(0xF8)://CLC
        {
            control_reg_flag = control_reg_flag & 0xFFFE;
            break;
        }

        case(0xF9)://STC
        {
            control_reg_flag = control_reg_flag | 0x1;
            break;
        }

        case(0xFA)://CLI
        {
            if_flag = 0;
            //control_reg_flag = control_reg_flag & 0xFDFF;
            break;
        }

        case(0xFB)://STI
        {
            if_flag = 1;
            //control_reg_flag = control_reg_flag | 0x200;
            break;
        }

        case(0xFC)://CLD
        {
            control_reg_flag = control_reg_flag & 0xFBFF;
            break;
        }

        case(0xFD)://STD
        {
            control_reg_flag = control_reg_flag | 0x400;
            break;
        }

        case(0xFE)://group Eb
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_8bit = CalculateRM(mod_byte, opcode);
            switch(group_index)
            {
            case(0x00)://INC Eb
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %2;\n\t"
                    "POPFW;\n\t"
                    "INCB %0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :      "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://DEC Eb
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %2;\n\t"
                    "POPFW;\n\t"
                    "DECB %0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_8bit), "=r"(control_reg_flag) /* output */
                    :      "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            default:
                break;
            }
            break;
        }

        case(0xFF)://group Ev
        {
            mod_byte = ReadData8InExe();
            uint8_t group_index = (mod_byte >> 3) & 0x7;
            opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
            switch(group_index)
            {
            case(0x00)://INC Ev
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %2;\n\t"
                    "POPFW;\n\t"
                    "INCW %0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :       "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x01)://DEC Ev
            {
                control_reg_flag &= 0xFEFF;
                __asm__
                (
                    "PUSHW %2;\n\t"
                    "POPFW;\n\t"
                    "DECW %0;\n\t"
                    "PUSHF;\n\t"
                    "POP %%EAX;\n\t"
                    "MOVW %%AX,%1;\n\t"
                    :"+r"(*opt1_16bit), "=r"(control_reg_flag) /* output */
                    :       "r"(control_reg_flag) /* input */
                    :"eax"
                );
                break;
            }

            case(0x02)://CALL Ev
            {
                Push(control_reg_ip);
                control_reg_ip = *opt1_16bit;
                break;
            }

            case(0x03)://CALL Mp
            {
                Push(seg_reg_cs);
                Push(control_reg_ip);
                control_reg_ip = *opt1_16bit;
                seg_reg_cs = *(opt1_16bit + 1);
                break;
            }

            case(0x04)://JMP Ev
            {
                control_reg_ip = *opt1_16bit;
                break;
            }

            case(0x05)://JMP Mp
            {
                control_reg_ip = *opt1_16bit;
                seg_reg_cs = *(opt1_16bit + 1);
                break;
            }

            case(0x06)://PUSH Ev
            {
                Push(*opt1_16bit);
                break;
            }

            default:
                break;
            }
            break;
        }
        }
        // seg_reg_replace_ds = &seg_reg_ds;
        // seg_reg_replace_ss = &seg_reg_ss;

        rep = 0;
        continue_check = 1;

        // if(count_code >= 500000 && count_code <= 800000)
        {
    ///        if(count_code <= 7000000 && count_code > 600000)
    ///            printdebug(opcode, count_code, ip_tmp);
    ///        //printf("%x\n",opcode);
    ///        fflush(stdout);
        }
    }
}

bool Cpu::LoadBIOS(string filename)
{
    long l, m, filesize;
    ifstream bios(filename.c_str(), ios::in | ios::binary);
    if(bios.is_open())
    {
        l = bios.tellg();
        bios.seekg(0, ios::end);
        m = bios.tellg();
        filesize = m - l;
        bios.seekg(0, ios::beg);
        bios.read((char *)(&ram[0x100000 - filesize]), filesize);
        //   std::cout << "Load " << filename << " at " << 0x100000-filesize;
        bios.close();
        return true;
    }
    return false;
}

bool Cpu::LoadRom(uint32_t addr, string filename)
{
    long l, m, filesize;
    ifstream rom(filename.c_str(), ios::in | ios::binary);
    if(rom.is_open())
    {
        l = rom.tellg();
        rom.seekg(0, ios::end);
        m = rom.tellg();
        filesize = m - l;
        rom.seekg(0, ios::beg);
        rom.read((char *)(&ram[addr]), filesize);
        rom.close();
        return true;
    }
    return false;
}

/*debug function (print registers)*/
void Cpu::printdebug(uint8_t opcode, uint32_t count, uint32_t ip_addr)
{
    uint8_t of, df, sf, zf, af, pf, cf;
    of = (control_reg_flag >> 11) & 0x1;
    df = (control_reg_flag >> 10) & 0x1;
    //If = (control_reg_flag >> 9) & 0x1;
    //tf = (control_reg_flag >> 8) & 0x1;
    sf = (control_reg_flag >> 7) & 0x1;
    zf = (control_reg_flag >> 6) & 0x1;
    af = (control_reg_flag >> 4) & 0x1;
    pf = (control_reg_flag >> 2) & 0x1;
    cf = (control_reg_flag >> 0) & 0x1;

    /*ax,bx,cx,dx sp,bp,si,di,ip,flag,cs,ds,ss,es*/
    printf("After %ld opcode :%x\n", count, opcode);
    ip_addr = ip_addr & 0xFFFFF;
    printf("%x %x %x %x %x %x %x %x\n", ram[ip_addr], ram[ip_addr + 1], ram[ip_addr + 2], ram[ip_addr + 3], ram[ip_addr + 4], ram[ip_addr + 5], ram[ip_addr + 6], ram[ip_addr + 7]);
    printf("CS:%x IP:%x DS:%x SS:%x ES:%x\n", GetCS(), GetIP(), GetDS(), GetSS(), GetES());
    printf("AX:%x BX:%x CX:%x DX:%x\n", GetAX(), GetBX(), GetCX(), GetDX());
    printf("SP:%x BP:%x SI:%x DI:%x\n", GetSP(), GetBP(), GetSI(), GetDI());
    printf("next opcode :%x\n", ram[GetCS() * 16 + GetIP()]);
    printf("of:%x df:%x if:%x tf:%x sf:%x zf:%x af:%x pf:%x cf:%x\n", of, df, if_flag, tf_flag, sf, zf, af, pf, cf);
    printf("\n======================\n");
    fflush(stdout);
}
