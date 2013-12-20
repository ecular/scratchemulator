#include "cpu.h"

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


/*set reg*/
inline void Cpu::SetAX(uint16_t value)
{
    universal_reg_ax = value & 0xFFFF;
}
inline void Cpu::SetAH(uint8_t value)
{
    universal_reg_ax = (universal_reg_ax & 0x00FF) | ((value & 0x00FF) << 8);
}
inline void Cpu::SetAL(uint8_t value)
{
    universal_reg_ax = (universal_reg_ax & 0xFF00) | (value & 0x00FF);
}

inline void Cpu::SetBX(uint16_t value)
{
    universal_reg_bx = value & 0xFFFF;
}
inline void Cpu::SetBH(uint8_t value)
{
    universal_reg_bx = (universal_reg_bx & 0x00FF) | ((value & 0x00FF) << 8);
}
inline void Cpu::SetBL(uint8_t value)
{
    universal_reg_bx = (universal_reg_bx & 0xFF00) | (value & 0x00FF);
}

inline void Cpu::SetCX(uint16_t value)
{
    universal_reg_cx = value & 0xFFFF;
}
inline void Cpu::SetCH(uint8_t value)
{
    universal_reg_cx = (universal_reg_cx & 0x00FF) | ((value & 0x00FF) << 8);
}
inline void Cpu::SetCL(uint8_t value)
{
    universal_reg_cx = (universal_reg_cx & 0xFF00) | (value & 0x00FF);
}

inline void Cpu::SetDX(uint16_t value)
{
    universal_reg_dx = value & 0xFFFF;
}
inline void Cpu::SetDH(uint8_t value)
{
    universal_reg_dx = (universal_reg_dx & 0x00FF) | ((value & 0x00FF) << 8);
}
inline void Cpu::SetDL(uint8_t value)
{
    universal_reg_dx = (universal_reg_dx & 0xFF00) | (value & 0x00FF);
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
    return universal_reg_ax >> 8;
}
inline uint8_t Cpu::GetAL()
{
    return static_cast<uint8_t>(universal_reg_ax);
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
    return ram[location] + (ram[location + 1] << 8);
}

