#ifndef EMU_VIDEO_H
#define EMU_VIDEO_H

#include <stdint.h>

class Cpu;

class Video {
    friend class timer;
private:
    Cpu *cpu;
    uint8_t CRT_Control_Reg[25], Sequence_Reg[5], Graphic_Control_Reg[9], Attribute_Control_Reg[20], Feature_Control_Reg, Input_Status_Reg1;
    uint32_t CGApalette[16], Table_Data_Reg[256], VGA_Memory[262144], Video_Buffer_Address;
    uint8_t CRT_Control_Reg_Index, Sequence_Reg_Index, Graphic_Control_Reg_Index, Attribute_Control_Reg_Index;
    uint8_t DAC_Status_Reg, Table_Index_Reg_Read, Table_Index_Reg_Write, Plane_Lock[4];
    uint8_t Attribute_Control_Reg_Flag/*0:index.1:data*/, Graphic_Mode_Flag, Colorful_Flag, Mode_Change_Flag;
    uint16_t cursor_x, cursor_y, width, high;
    uint8_t columns, rows;
    uint8_t Table_Read_Step, Table_Write_Step; /*1:Red 2:Green 3:Blue*/
    uint8_t CGA_ascii_table[0x8000];
    uint16_t Cursor_Position;

    uint8_t rotate_right(uint8_t value, uint8_t shift_bits);
    uint8_t logic_operate(uint8_t value, uint8_t plane_lock);
public:
    friend class Cpu;
    friend class Display;
    uint8_t screen_update_flag, Video_Mode;
    Video();
    uint8_t read_video(uint16_t port_num);
    void write_video(uint16_t port_num, uint8_t value);
    uint8_t read_video_memory(uint32_t addr_offset);
    void write_video_memory(uint32_t addr_offset, uint8_t value);
    void interrupt();
    void setcpu(Cpu *);
};

#endif
