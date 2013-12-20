#ifndef EMU_CPU_H_
#define EMU_CPU_H_

#include <stdint.h>

class Cpu {
    /* 声明次序
     * 1) typedefs 和 enums;
     * 2) 常量;
     * 3) 构造函数;
     * 4) 析构函数;
     * 5) 成员函数,含静态成员函数;
     * 6) 数据成员,含静态数据成员。
     * */
public:

    //Cpu();
    //~Cpu();
    int Init(unsigned int ram_size);
    int Reset();

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

    /*write ram*/
    void WriteRam8(unsigned int location, uint8_t value);
    void WriteRam16(unsigned int location, uint16_t value);

    /*read ram*/
    uint8_t ReadRam8(unsigned int location);
    uint16_t ReadRam16(unsigned int location);

private:

    uint8_t *ram;

    uint16_t universal_reg_ax, universal_reg_bx, universal_reg_cx, universal_reg_dx,
             universal_reg_sp, universal_reg_bp, universal_reg_si, universal_reg_di;
    //uint8_t universal_reg_ah, universal_reg_al, universal_reg_bh, universal_reg_bl,
    //      universal_reg_ch, universal_reg_cl, universal_reg_dh, universal_reg_dl;
    uint16_t control_reg_ip, control_reg_flag;
    uint16_t seg_reg_cs, seg_reg_ds, seg_reg_ss, seg_reg_es;

};

#endif