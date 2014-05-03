#include "port_handle.h"
#include <stdint.h>
#include "8259a.h"
#include "8253.h"

port_handle::port_handle()
{
    //init
}

void port_handle::seti8259a(Interrupt_Controller_8259a *arg)
{
    i8259a = arg;
}

void port_handle::seti8253(Interval_Timer_8253 *arg)
{
    i8253 = arg;
}

void port_handle::setvideo(Video *arg)
{
    video = arg;
}

void port_handle::port_handle_write8(uint16_t port_num, uint8_t value)
{
    switch(port_num)
    {
    /*8259a Interrupt_Controller_8259a*/
    case(0x20):
    case(0x21):
    {
        i8259a->write_8259a(port_num, value);
        break;
    }
    /*8253 Interval_Timer_8253*/
    case(0x40):
    case(0x41):
    case(0x42):
    case(0x43):
    {
        i8253->write_8253(port_num, value);
        break;
    }
    /*video card*/
    case(0x3B0):
    case(0x3B1):
    case(0x3B2):
    case(0x3B3):
    case(0x3B4):
    case(0x3B5):
    case(0x3B6):
    case(0x3B7):
    case(0x3B8):
    case(0x3B9):
    case(0x3BA):
    case(0x3BB):
    case(0x3BC):
    case(0x3BD):
    case(0x3BE):
    case(0x3BF):
    case(0x3C0):
    case(0x3C1):
    case(0x3C2):
    case(0x3C3):
    case(0x3C4):
    case(0x3C5):
    case(0x3C6):
    case(0x3C7):
    case(0x3C8):
    case(0x3C9):
    case(0x3CA):
    case(0x3CB):
    case(0x3CC):
    case(0x3CD):
    case(0x3CE):
    case(0x3CF):
    case(0x3D0):
    case(0x3D1):
    case(0x3D2):
    case(0x3D3):
    case(0x3D4):
    case(0x3D5):
    case(0x3D6):
    case(0x3D7):
    case(0x3D8):
    case(0x3D9):
    case(0x3DA):
    {
        video->write_video(port_num, value);
        break;
    }
    }
}

uint8_t port_handle::port_handle_read8(uint16_t port_num)
{
    switch(port_num)
    {
    /*8259a Interrupt_Controller_8259a*/
    case(0x20):
    case(0x21):
    {
        return i8259a->read_8259a(port_num);
    }
    /*8253 Interval_Timer_8253*/
    case(0x40):
    case(0x41):
    case(0x42):
    case(0x43):
    {
        return i8253->read_8253(port_num);
    }
    /*video card*/
    case(0x3B0):
    case(0x3B1):
    case(0x3B2):
    case(0x3B3):
    case(0x3B4):
    case(0x3B5):
    case(0x3B6):
    case(0x3B7):
    case(0x3B8):
    case(0x3B9):
    case(0x3BA):
    case(0x3BB):
    case(0x3BC):
    case(0x3BD):
    case(0x3BE):
    case(0x3BF):
    case(0x3C0):
    case(0x3C1):
    case(0x3C2):
    case(0x3C3):
    case(0x3C4):
    case(0x3C5):
    case(0x3C6):
    case(0x3C7):
    case(0x3C8):
    case(0x3C9):
    case(0x3CA):
    case(0x3CB):
    case(0x3CC):
    case(0x3CD):
    case(0x3CE):
    case(0x3CF):
    case(0x3D0):
    case(0x3D1):
    case(0x3D2):
    case(0x3D3):
    case(0x3D4):
    case(0x3D5):
    case(0x3D6):
    case(0x3D7):
    case(0x3D8):
    case(0x3D9):
    case(0x3DA):
    {
        return video->read_video(port_num);
    }
    }
    return 0x00;
}

void port_handle::port_handle_write16(uint8_t port_num, uint16_t value)
{
    port_handle_write8(port_num, value & 0x00FF);
    port_handle_write8(port_num, value >> 4);
}

uint16_t port_handle::port_handle_read16(uint8_t port_num)
{
    uint16_t result;
    result = port_handle_read8(port_num);
    result |= ((uint16_t)(port_handle_read8(port_num + 1))) << 8;
    return result;
}
