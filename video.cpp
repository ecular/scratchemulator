#include "video.h"
#include <stdint.h>
#include <string.h>
#include "cpu.h"

Video::Video()
{
    screen_update_flag = 1;
    Graphic_Mode_Flag = 0;
    for(int i = 0; i < 262144; ++i)
        VGA_Memory[i] = 0;
    columns = 80;
    rows = 25;
    width = 640;
    high = 400;

    // ifstream cgaasc("./asciivga.dat", ios::in | ios::binary);
    // if(cgaasc.is_open())
    // {
    //     cgaasc.read((char *)(&CGA_ascii_table[0]), 0x8000);
    //     cgaasc.close();
    // }
    FILE *binFile = fopen("asciivga.dat", "rb");
    if(binFile)
    {
        fread((char *)(&CGA_ascii_table[0]), 1, 0x8000, binFile);
    }
    else
    {
        printf("can't open file.");
    }



    CGApalette[0] = 0x000000; /*black*/
    CGApalette[1] = 0x0000AA; /*blue*/
    CGApalette[2] = 0x00AA00; /*green*/
    CGApalette[3] = 0x00AAAA; /*cyan*/
    CGApalette[4] = 0xAA0000; /*red*/
    CGApalette[5] = 0xAA00AA; /*magenta*/
    CGApalette[6] = 0xAA5500; /*brown*/
    CGApalette[7] = 0xAAAAAA; /*light gray*/
    CGApalette[8] = 0x555555; /*gray*/
    CGApalette[9] = 0x5555FF; /*light blue*/
    CGApalette[10] = 0x55FF55; /*light green*/
    CGApalette[11] = 0x55FFFF; /*light cyan*/
    CGApalette[12] = 0xFF5555; /*light red*/
    CGApalette[13] = 0xFF55FF; /*light magenta*/
    CGApalette[14] = 0xFFFF55; /*yellow*/
    CGApalette[15] = 0xFFFFFF; /*white*/
}

inline uint8_t Video::rotate_right(uint8_t value, uint8_t shift_bits)
{
    return (value >> shift_bits) | ((value << (8 - shift_bits)) & 0xFF);
}

inline uint8_t Video::logic_operate(uint8_t value, uint8_t plane_lock)
{
    switch((Graphic_Control_Reg[3] >> 3) & 0x3)
    {
    case(0):/*driect return*/
    {
        return value;
    }
    case(1):/*AND*/
    {
        value &= plane_lock;
        return value;
    }
    case(2):/*OR*/
    {
        value |= plane_lock;
        return value;
    }
    case(3):/*XOR*/
    {
        value ^= plane_lock;
        return value;
    }
    }
    return value;
}


void Video::write_video(uint16_t port_num, uint8_t value)
{
    static uint32_t Color_Data_Tmp;
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
        case(0xE):/*cursor position high bits*/
        {
            Cursor_Position = ((Cursor_Position & 0xFF) | (value << 8));
            break;
        }
        case(0xF):/*cursor positon low bits*/
        {
            Cursor_Position = ((Cursor_Position & 0xFF00) | value);
            break;
        }
        case(0x6):/*Vertical total*/
        {
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

uint8_t Video::read_video(uint16_t port_num)
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
    case(0x3DA):/*Input status reg_1*/
    {
        Attribute_Control_Reg_Flag = 0;
        return Input_Status_Reg1;
    }
    }
    return 0x00;
}


void Video::write_video_memory(uint32_t addr_offset, uint8_t value)
{
    uint8_t write_value;
    uint32_t plane_size = 0x10000;
    switch(Graphic_Control_Reg[5] & 0x3)
    {
    case(0):/*write mode 0*/
    {
        /*rotate shift*/
        value = rotate_right(value, Graphic_Control_Reg[3] & 0x7);

        if(Sequence_Reg[2] & 0x1) /*Plane 0*/
        {
            if(Graphic_Control_Reg[1] & 0x1)
            {
                if(Graphic_Control_Reg[0] & 0x1)
                    write_value = 0xFF;
                else
                    write_value = 0;
            }
            else
            {
                write_value = logic_operate(value, Plane_Lock[0]);
            }
            write_value = (Graphic_Control_Reg[8] & write_value) | (~Graphic_Control_Reg[8] & Plane_Lock[0]);
            VGA_Memory[addr_offset] = write_value;
        }

        if(Sequence_Reg[2] & 0x2) /*Plane 1*/
        {
            if(Graphic_Control_Reg[1] & 0x2)
            {
                if(Graphic_Control_Reg[0] & 0x2)
                    write_value = 0xFF;
                else
                    write_value = 0;
            }
            else
            {
                write_value = logic_operate(value, Plane_Lock[1]);
            }
            write_value = (Graphic_Control_Reg[8] & write_value) | (~Graphic_Control_Reg[8] & Plane_Lock[1]);
            VGA_Memory[addr_offset + plane_size] = write_value;
        }

        if(Sequence_Reg[2] & 0x4) /*Plane 2*/
        {
            if(Graphic_Control_Reg[1] & 0x4)
            {
                if(Graphic_Control_Reg[0] & 0x4)
                    write_value = 0xFF;
                else
                    write_value = 0;
            }
            else
            {
                write_value = logic_operate(value, Plane_Lock[2]);
            }
            write_value = (Graphic_Control_Reg[8] & write_value) | (~Graphic_Control_Reg[8] & Plane_Lock[2]);
            VGA_Memory[addr_offset + plane_size * 2] = write_value;
        }

        if(Sequence_Reg[2] & 0x8) /*Plane 3*/
        {
            if(Graphic_Control_Reg[1] & 0x8)
            {
                if(Graphic_Control_Reg[0] & 0x8)
                    write_value = 0xFF;
                else
                    write_value = 0;
            }
            else
            {
                write_value = logic_operate(value, Plane_Lock[3]);
            }
            write_value = (Graphic_Control_Reg[8] & write_value) | (~Graphic_Control_Reg[8] & Plane_Lock[3]);
            VGA_Memory[addr_offset + plane_size * 3] = write_value;
        }
        break;
    }

    case(1):/*write mode 1*/
    {
        if(Sequence_Reg[2] & 0x1) /*Plane 0*/
        {
            VGA_Memory[addr_offset] = Plane_Lock[0];
        }
        if(Sequence_Reg[2] & 0x2) /*Plane 1*/
        {
            VGA_Memory[addr_offset + plane_size * 1] = Plane_Lock[1];
        }
        if(Sequence_Reg[2] & 0x4) /*Plane 2*/
        {
            VGA_Memory[addr_offset + plane_size * 2] = Plane_Lock[2];
        }
        if(Sequence_Reg[2] & 0x8) /*Plane 3*/
        {
            VGA_Memory[addr_offset + plane_size * 3] = Plane_Lock[3];
        }
        break;
    }

    case(2):/*write mode 2*/
    {
        if(Sequence_Reg[2] & 0x1) /*Plane 0*/
        {
            if(value & 0x1)
                write_value = 0xFF;
            else
                write_value = 0;
            logic_operate(write_value, Plane_Lock[0]);
            write_value = (Graphic_Control_Reg[8] & write_value) | (~Graphic_Control_Reg[8] & Plane_Lock[0]);
            VGA_Memory[addr_offset + plane_size * 0] = write_value;
        }

        if(Sequence_Reg[2] & 0x2) /*Plane 1*/
        {
            if(value & 0x2)
                write_value = 0xFF;
            else
                write_value = 0;
            logic_operate(write_value, Plane_Lock[1]);
            write_value = (Graphic_Control_Reg[8] & write_value) | (~Graphic_Control_Reg[8] & Plane_Lock[1]);
            VGA_Memory[addr_offset + plane_size * 1] = write_value;
        }

        if(Sequence_Reg[2] & 0x4) /*Plane 2*/
        {
            if(value & 0x4)
                write_value = 0xFF;
            else
                write_value = 0;
            logic_operate(write_value, Plane_Lock[2]);
            write_value = (Graphic_Control_Reg[8] & write_value) | (~Graphic_Control_Reg[8] & Plane_Lock[2]);
            VGA_Memory[addr_offset + plane_size * 2] = write_value;
        }

        if(Sequence_Reg[2] & 0x8) /*Plane 3*/
        {
            if(value & 0x8)
                write_value = 0xFF;
            else
                write_value = 0;
            logic_operate(write_value, Plane_Lock[3]);
            write_value = (Graphic_Control_Reg[8] & write_value) | (~Graphic_Control_Reg[8] & Plane_Lock[3]);
            VGA_Memory[addr_offset + plane_size * 3] = write_value;
        }
        break;
    }
    case(3):/*write mode 3*/
    {
        /*rotate shift*/
        value = rotate_right(value, Graphic_Control_Reg[3] & 0x7);
        uint8_t bit_mask = value & Graphic_Control_Reg[8];

        if(Sequence_Reg[2] & 0x1) /*Plane 0*/
        {
            if(Graphic_Control_Reg[0] & 0x1)
                write_value = 0xFF;
            else
                write_value = 0;
            VGA_Memory[addr_offset + plane_size * 0] = write_value & bit_mask;
        }

        if(Sequence_Reg[2] & 0x2) /*Plane 1*/
        {
            if(Graphic_Control_Reg[0] & 0x2)
                write_value = 0xFF;
            else
                write_value = 0;
            VGA_Memory[addr_offset + plane_size * 1] = write_value & bit_mask;
        }

        if(Sequence_Reg[2] & 0x4) /*Plane 2*/
        {
            if(Graphic_Control_Reg[0] & 0x4)
                write_value = 0xFF;
            else
                write_value = 0;
            VGA_Memory[addr_offset + plane_size * 2] = write_value & bit_mask;
        }

        if(Sequence_Reg[2] & 0x8) /*Plane 3*/
        {
            if(Graphic_Control_Reg[0] & 0x8)
                write_value = 0xFF;
            else
                write_value = 0;
            VGA_Memory[addr_offset + plane_size * 3] = write_value & bit_mask;
        }
        break;
    }
    }
}

/*may have some error.may no use*/
uint8_t Video::read_video_memory(uint32_t addr_offset)
{
    uint32_t plane_size = 0x10000;
    Plane_Lock[0] = VGA_Memory[addr_offset + plane_size * 0];
    Plane_Lock[1] = VGA_Memory[addr_offset + plane_size * 1];
    Plane_Lock[2] = VGA_Memory[addr_offset + plane_size * 2];
    Plane_Lock[3] = VGA_Memory[addr_offset + plane_size * 3];
    switch(Graphic_Control_Reg[4] & 0x3)
    {
    case(0):
    {
        Plane_Lock[0] = VGA_Memory[addr_offset + plane_size * 0];
        return Plane_Lock[0];
    }
    case(1):
    {
        Plane_Lock[1] = VGA_Memory[addr_offset + plane_size * 1];
        return Plane_Lock[1];
    }
    case(2):
    {
        Plane_Lock[2] = VGA_Memory[addr_offset + plane_size * 2];
        return Plane_Lock[2];
    }
    case(3):
    {
        Plane_Lock[3] = VGA_Memory[addr_offset + plane_size * 3];
        return Plane_Lock[3];
    }
    }
    return 0xFF;
}

void Video::interrupt()
{
    switch(cpu->GetAH())
    {
    case(0):/*set video mode*/
    {
        //               printf("Set video mode %x\n",cpu->GetAL()&0x7F);
        switch(cpu->GetAL() & 0x7F)
        {
        case(0):/*Text 40x25 16gray 0xB8000*/
        {
            Video_Buffer_Address = 0xB8000;
            columns = 40;
            rows = 25;
            Colorful_Flag = 0;
            Graphic_Mode_Flag = 0;/*text mode*/
            for(uint32_t tmp_addr = Video_Buffer_Address; tmp_addr < Video_Buffer_Address + 0x4000; tmp_addr += 2)
            {
                cpu->ram[tmp_addr] = 0;/*ascii*/
                cpu->ram[tmp_addr + 1] = 7;/*text attribute*/
            }
            break;
        }

        case(1):/*Text 40x25 16 fore/8 back  0xB8000*/
        {
            Video_Buffer_Address = 0xB8000;
            columns = 40;
            rows = 25;
            Colorful_Flag = 1;/*colorful*/
            Graphic_Mode_Flag = 0;/*text mode*/
            for(uint32_t tmp_addr = Video_Buffer_Address; tmp_addr < Video_Buffer_Address + 0x4000; tmp_addr += 2)
            {
                cpu->ram[tmp_addr] = 0;/*ascii*/
                cpu->ram[tmp_addr + 1] = 7;/*text attribute*/
            }
            break;
        }

        case(2):/*Text  80 x 25 16 gray  0xB8000*/
        {
            Video_Buffer_Address = 0xB8000;
            columns = 80;
            rows = 25;
            Colorful_Flag = 0;/*colorful*/
            Graphic_Mode_Flag = 0;/*text mode*/
            for(uint32_t tmp_addr = Video_Buffer_Address; tmp_addr < Video_Buffer_Address + 0x4000; tmp_addr += 2)
            {
                cpu->ram[tmp_addr] = 0;/*ascii*/
                cpu->ram[tmp_addr + 1] = 7;/*text attribute*/
            }
            break;
        }

        case(3):/*Text  80 x 25 16 fore/8 back  0xB8000*/
        {
            Video_Buffer_Address = 0xB8000;
            columns = 80;
            rows = 25;
            Colorful_Flag = 1;/*colorful*/
            Graphic_Mode_Flag = 0;/*text mode*/
            for(uint32_t tmp_addr = Video_Buffer_Address; tmp_addr < Video_Buffer_Address + 0x4000; tmp_addr += 2)
            {
                cpu->ram[tmp_addr] = 0;/*ascii*/
                cpu->ram[tmp_addr + 1] = 7;/*text attribute*/
            }
            break;
        }

        case(4):/*Text  80 x 25 16 fore/8 back  0xB8000*/
        {
            Video_Buffer_Address = 0xB8000;
            columns = 80;
            rows = 25;
            Colorful_Flag = 1;/*colorful*/
            Graphic_Mode_Flag = 0;/*text mode*/
            for(uint32_t tmp_addr = Video_Buffer_Address; tmp_addr < Video_Buffer_Address + 0x4000; tmp_addr += 2)
            {
                cpu->ram[tmp_addr] = 0;/*ascii*/
                cpu->ram[tmp_addr + 1] = 7;/*text attribute*/
            }
            break;
        }

        case(0x0D):/*320x200 16-color 0xA0000*/
        case(0x12):/*640x480 16-color 0xA0000*/
        case(0x13):/*320x200 256-color 0xA0000*/
        {
            Video_Buffer_Address = 0xA0000;
            columns = 40;
            rows = 25;
            Colorful_Flag = 1;/*colorful*/
            Graphic_Mode_Flag = 1;/*graphic mode*/
            memset(cpu->ram + Video_Buffer_Address, 0, 0xFFFF);
        }
        }
        Video_Mode = cpu->GetAL() & 0x7F;
        cursor_x = cursor_y = 0;
        switch(Video_Mode)
        {
        case(0x12):
        {
            width = 640;
            high = 480;
            break;
        }
        case(0x0D):
        case(0x13):
        {
            width = 320;
            high = 200;
            break;
        }
        default:
        {
            width = 640;
            high = 400;
        }
        }
        Mode_Change_Flag = 1;
        break;
    }

    case(0x10):/*SET DAC REGISTER*/
    {
        switch(cpu->GetAL())
        {
        case(0x10):/*SET INDIVIDUAL DAC REGISTER (VGA/MCGA)*/
        {
            Table_Data_Reg[cpu->GetBX()] = (((cpu->GetDH() & 0x3F) << 2) | (((cpu->GetCL() & 0x3F) << 2) << 8) | (((cpu->GetCH() & 0x3F) << 2) << 16));
            break;
        }
        case(0x12):/*SET BLOCK OF DAC REGISTERS (VGA/MCGA)*/
        {
            uint32_t start_index = cpu->GetBX();
            uint32_t start_addr = (cpu->GetES() << 4) + cpu->GetDX();
            for(uint32_t i = 0; i < cpu->GetCX(); ++i)
            {
                Table_Data_Reg[start_index + i] = (((cpu->ReadRam8(start_addr) & 0x3F) << 2) | (((cpu->ReadRam8(start_addr + 1) & 0x3F) << 2) << 8) | (((cpu->ReadRam8(start_addr + 2) & 0x3F) << 2) << 16));
                start_addr += 3;
            }
            break;
        }
        }
        break;
    }
    case(0x1A):/*GET DISPLAY COMBINATION CODE (PS,VGA/MCGA)*/
    {
        cpu->SetAL(0x1A);
        cpu->SetBL(0x8);
        break;
    }
    }
}

void Video::setcpu(Cpu *arg)
{
    cpu = arg;
}
