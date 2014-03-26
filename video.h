#ifndef EMU_VIDEO_H
#define EMU_VIDEO_H

#include <stdint.h>

class video {
private:
    uint8_t CRT_Control_Reg[25], Sequence_Reg[5], Graphic_Control_Reg[9], Attribute_Control_Reg[20];
    uint32_t Table_Data_Reg[256];
    uint8_t CRT_Control_Reg_Index, Sequence_Reg_Index, Graphic_Control_Reg_Index, Attribute_Control_Reg_Index;
    uint8_t DAC_Status_Reg, Table_Index_Reg_Read, Table_Index_Reg_Write;
    uint8_t Attribute_Control_Reg_Flag;/*0:index.1:data*/
    uint16_t cursor_x, cursor_y;
    uint8_t columns, rows;
public:
    friend class Cpu;
    video();
    uint8_t read_video(uint16_t port_num);
    void write_video(uint16_t port_num, uint8_t value);
};

#endif
