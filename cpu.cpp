#include "cpu.h"

#include <stdio.h>

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
    control_reg_ip += steps;
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
inline void Cpu::WriteRam8(unsigned int location, uint8_t value)
{
    ram[location] = value & 0x00FF;
}
inline void Cpu::WriteRam16(unsigned int location, uint16_t value)
{
    WriteRam8(location++, value & 0x00FF);
    WriteRam8(location, value >> 8);
}

/*read ram*/
inline uint8_t Cpu::ReadRam8(unsigned int location)
{
    return ram[location];
}
inline uint16_t Cpu::ReadRam16(unsigned int location)
{
    return ram[location] | (ram[location + 1] << 8);
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
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx + universal_reg_si];
        case(0x01):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx + universal_reg_di];
        case(0x02):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bp + universal_reg_si];
        case(0x03):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bp + universal_reg_di];
        case(0x04):
            return &ram[(*seg_reg_replace << 4) + universal_reg_si];
        case(0x05):
            return &ram[(*seg_reg_replace << 4) + universal_reg_di];
        case(0x06):
            return &ram[(*seg_reg_replace << 4) + ReadData16InExe()];
        case(0x07):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx];
        }

    case(0x1):
        switch(rm)
        {
        case(0x00):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx + universal_reg_si + ReadData8InExe()];
        case(0x01):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx + universal_reg_di + ReadData8InExe()];
        case(0x02):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bp + universal_reg_si + ReadData8InExe()];
        case(0x03):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bp + universal_reg_di + ReadData8InExe()];
        case(0x04):
            return &ram[(*seg_reg_replace << 4) + universal_reg_si + ReadData8InExe()];
        case(0x05):
            return &ram[(*seg_reg_replace << 4) + universal_reg_di + ReadData8InExe()];
        case(0x06):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bp + ReadData8InExe()];
        case(0x07):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx + ReadData8InExe()];
        }

    case(0x2):
        switch(rm)
        {
        case(0x00):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx + universal_reg_si + ReadData16InExe()];
        case(0x01):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx + universal_reg_di + ReadData16InExe()];
        case(0x02):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bp + universal_reg_si + ReadData16InExe()];
        case(0x03):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bp + universal_reg_di + ReadData16InExe()];
        case(0x04):
            return &ram[(*seg_reg_replace << 4) + universal_reg_si + ReadData16InExe()];
        case(0x05):
            return &ram[(*seg_reg_replace << 4) + universal_reg_di + ReadData16InExe()];
        case(0x06):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bp + ReadData16InExe()];
        case(0x07):
            return &ram[(*seg_reg_replace << 4) + universal_reg_bx + ReadData16InExe()];
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

int Cpu::Init(unsigned int ram_size)
{
    ram = new uint8_t[ram_size];

    Reset();
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
    return 0;
}

void Cpu::Exec()
{
    uint8_t opcode;
    uint8_t mod_byte;
    uint8_t *opt1_8bit, *opt2_8bit;
    uint16_t *opt1_16bit, *opt2_16bit;
    seg_reg_replace = &seg_reg_ds;
    opcode = ReadData8InExe();
    switch(opcode)
    {
    /*seg prefix
     * case(0x26):
     seg_reg_replace = &seg_reg_es;//es prefix
     case(0x2E):
     seg_reg_replace = &seg_reg_cs;//cs prefix
     case(0x36):
     seg_reg_replace = &seg_reg_ss;//ss prefix
     case(0x3E):
     seg_reg_replace = &seg_reg_ds;//ds prefix
     */
    case(0x00)://ADD Eb Gb
    {
        mod_byte = ReadData8InExe();
        opt1_8bit = CalculateRM(mod_byte, opcode);
        opt2_8bit = CalculateReg8(mod_byte);
        *opt1_8bit = *opt1_8bit + *opt2_8bit;
        break;
    }

    case(0x01)://ADD Ev Gv
    {
        mod_byte = ReadData8InExe();
        opt1_16bit = reinterpret_cast<uint16_t  *>(CalculateRM(mod_byte, opcode));
        opt2_16bit = CalculateReg16(mod_byte);
        *opt1_16bit = *opt1_16bit + *opt2_16bit;
        break;
    }

    case(0x02)://ADD Gb Eb
    {
        mod_byte = ReadData8InExe();
        opt1_8bit = CalculateReg8(mod_byte);
        opt2_8bit = CalculateRM(mod_byte, opcode);
        *opt1_8bit = *opt1_8bit + *opt2_8bit;
        break;
    }
    }
}
