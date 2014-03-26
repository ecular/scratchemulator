#include "video.h"
#include <stdint.h>

video::video()
{
    columns = 80;
    rows = 25;
}

void video::write_video(uint16_t port_num, uint8_t value)
{
    static uint32_t Color_Data_Tmp;
    static uint16_t Cursor_Position;
    switch(port_num)
    {
    case(0x3D4):/*CRT Control reg index*/
    {
        CRT_Control_Reg_Index = value & 0xFF;
        break;
    }
    case(0x3D5):/*CRT Control reg*/
    {
        CRT_Control_Reg[CRT_Control_Reg_Index] = value & 0xFF;
        switch(CRT_Control_Reg_Index)
        {
        case(0x6):/*Vertical total*/
        {
            break;
        }
        case(0xE):/*cursor position high bits*/
        {
            Cursor_Position &= 0x00FF;
            Cursor_Position |= (value << 8);
            break;
        }
        case(0xF):/*cursor positon low bits*/
        {
            Cursor_Position &= 0xFF00;
            Cursor_Position |= value;
            break;
        }
        }
        cursor_y = Cursor_Position / columns;
        cursor_x = Cursor_Position % columns;
        break;
    }

    case(0x3C4):/*sequence reg index*/
    {
        Sequence_Reg_Index = value & 0xFF;
        break;
    }
    case(0x3C5):/*sequence reg*/
    {
        Sequence_Reg[Sequence_Reg_Index] = value & 0xFF;
        break;
    }

    case(0x3CE):/*graphic control reg index*/
    {
        Graphic_Control_Reg_Index = value & 0xFF;
        break;
    }
    case(0x3CF):/*graphic control reg*/
    {
        Graphic_Control_Reg[Graphic_Control_Reg_Index] = value & 0xFF;
        break;
    }

    case(0x3C0):/*attribute control reg only use I/O 0x3c0 for index & data*/
    {
        if(Attribute_Control_Reg_Flag)/*data*/
        {
            Attribute_Control_Reg[Attribute_Control_Reg_Index] = value & 0xFF;
            Attribute_Control_Reg_Flag = 0;
        }
        else/*index*/
        {
            Attribute_Control_Reg_Index = value & 0xFF;
            Attribute_Control_Reg_Flag = 1;
            break;
        }
    }

    case(0x3C7):/*table index reg(read)*/
    {
        DAC_Status_Reg = 0;/*set DAC status read*/
        Table_Index_Reg_Read = value & 0xFF;
        Table_Read_Step = 1;
        break;
    }

    case(0x3C8):/*table index reg(write)*/
    {
        DAC_Status_Reg = DAC_Status_Reg | 0x3;/*set DAC status write*/
        Table_Index_Reg_Write = value & 0xFF;
        Table_Write_Step = 1;
        break;
    }

    case(0x3C9):/*table date reg*/
    {
        switch(Table_Write_Step)
        {
        case(1):/*Red*/
        {
            Table_Write_Step++;
            Color_Data_Tmp = value << 2;
            break;
        }
        case(2):/*Green*/
        {
            Table_Write_Step++;
            Color_Data_Tmp = value << 10;
            break;
        }
        case(3):/*Blue*/
        {
            Table_Write_Step = 1;
            Color_Data_Tmp = value << 18;

            Color_Data_Tmp &= 0xFFFFFF;
            Table_Data_Reg[Table_Index_Reg_Write] = Color_Data_Tmp;
            Color_Data_Tmp &= 0;
            Table_Index_Reg_Write++;
            break;
        }
        }
        break;
    }

    case(0x3DA):/*Feature control register.to init Attribute_Control_Reg_Flag.Maybe no use*/
    {
        Feature_Control_Reg = value & 0xFF;
        Attribute_Control_Reg_Flag = 0;
        break;
    }
    }
}

uint8_t video::read_video(uint16_t port_num)
{
    switch(port_num)
    {
    case(0x3D5):/*CRT Control Reg*/
    {
        return CRT_Control_Reg[CRT_Control_Reg_Index];
    }
    case(0x3C5):/*Sequence Reg*/
    {
        return Sequence_Reg[Sequence_Reg_Index];
    }
    case(0x3CF):/*Graphic Control Reg*/
    {
        return Graphic_Control_Reg[Graphic_Control_Reg_Index];
    }
    case(0x3C0):/*Attribute Control Reg*/
    {
        return Attribute_Control_Reg[Attribute_Control_Reg_Index];
    }
    case(0x3C7):/*DAC status reg*/
    {
        return DAC_Status_Reg;
    }
    case(0x3C8):
    {
        return Table_Index_Reg_Write;
    }
    case(0x3C9):
    {
        switch(Table_Read_Step)
        {
        case(1):/*Red*/
        {
            Table_Read_Step++;
            return Table_Data_Reg[Table_Index_Reg_Read] >> 2 & 0x3F;
        }
        case(2):/*Green*/
        {
            Table_Read_Step++;
            return Table_Data_Reg[Table_Index_Reg_Read] >> 10 & 0x3F;
        }
        case(3):/*Blue*/
        {
            Table_Read_Step = 1;
            Table_Index_Reg_Read++;
            return Table_Data_Reg[Table_Index_Reg_Read] >> 18 & 0x3F;
        }
        }
    }
    case(0x3DA):/*Input status reg_0*/
    {
        Attribute_Control_Reg_Flag = 0;
        return Input_Status_Reg0;
    }
    }
    return 0;
}
