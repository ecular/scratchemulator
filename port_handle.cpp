#include "port_handle.h"
#include <stdint.h>
#include "8259a.h"
#include "8253.h"

port_handle::port_handle(Interrupt_Controller_8259a &i8259a_arg, Interval_Timer_8253 &i8253_arg): i8259a(i8259a_arg), i8253(i8253_arg)
{
    //init
}

void port_handle::port_handle_write8(uint8_t port_num, uint8_t value)
{
    switch(port_num)
    {
    /*8259a Interrupt_Controller_8259a*/
    case(0x20):
    case(0x21):
    {
        i8259a.write_8259a(port_num, value);
        break;
    }
    /*8253 Interval_Timer_8253*/
    case(0x40):
    case(0x41):
    case(0x42):
    case(0x43):
    {
        i8253.write_8253(port_num, value);
        break;
    }
    }
}

uint8_t port_handle::port_handle_read8(uint8_t port_num)
{
    switch(port_num)
    {
    /*8259a Interrupt_Controller_8259a*/
    case(0x20):
    case(0x21):
    {
        return i8259a.read_8259a(port_num);
    }
    /*8253 Interval_Timer_8253*/
    case(0x40):
    case(0x41):
    case(0x42):
    case(0x43):
    {
        return i8253.read_8253(port_num);
    }
    }
    return 0;
}

void port_handle::port_handle_write16(uint8_t port_num, uint16_t value)
{}

uint16_t port_handle::port_handle_read16(uint8_t port_num)
{
    return 0;
}
