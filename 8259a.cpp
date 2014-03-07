#include "8259a.h"

Interrupt_Controller_8259a::Interrupt_Controller_8259a()
{
    int i;
    IMR = 0xFF;
    IRR = 0;
    ISR = 0;
    for(i = 1; i < 5; i++)
        ICW[i] = 0;
    for(i = 1; i < 4; i++)
        OCW[i] = 0;
    init_order = 0;
    master_base_vector = 0;
}


void Interrupt_Controller_8259a::init_8259a()
{
}

uint8_t Interrupt_Controller_8259a::read_8259a(uint8_t port_num)
{
    switch(port_num)
    {
    case(0x20):
    {
        switch(OCW[3] & 0x3)
        {
        case(0x2):
            return IRR;/*read IRR*/
        case(0x3):
            return ISR;/*read ISR*/
        }
    }
    case(0x21): /* read IMR */
        return IMR;
    }
    return 0;
}

void Interrupt_Controller_8259a::write_8259a(uint8_t port_num, uint8_t value)
{
    switch(port_num)
    {
    case(0x20):/*for master ICW1 OCW2 OCW3*/
    {
        if(value & 0x10) /*ICW1*/
        {
            init_order = 1;
            ICW[init_order++] = value;
            IMR = 0;
            return;
        }

        if((value & 0x18) == 0) /* OCW2 */
        {
            OCW[2] = value;
            if((value >> 5) == 0x1)/*send a EOI*/
            {
                ISR = 0x0;
            }
            return;
        }

        if((value & 0x18) == 0x8) /* OCW3 */
        {
            OCW[3] = value;
            switch(value & 0x3)
            {
            case(0x2):
            {
                ;/*read IRR*/
                break;
            }
            case(0x3):
            {
                ;/*read ISR*/
                break;
            }
            }
            return;
        }
        break;
    }
    case(0x21):/*for master ICW2 ICW3 ICW4 OCW1*/
    {
        switch(init_order)
        {
        case(2):/* ICW2 */
        {
            if((value & 0x7) == 0)
            {
                ICW[init_order++] = value;
                master_base_vector = value;
            }
            return;
        }
        case(3):/* ICW3 */
        {
            if(ICW[1] & 0x2) /*if single 8259A,then skip ICW3*/
            {
                init_order++;
                return;
            }
            ICW[init_order++] = value;
            return;
        }
        case(4):/* ICW4 */
        {
            if(ICW[1] & 0x1)/*ICW4 need to set*/
                ICW[init_order++] = value;
            init_order++;
            return;
        }
        }
        IMR = value;/* init_order > 4 OCW1 */
        break;
    }
    }
}

void Interrupt_Controller_8259a::trigger_int(uint8_t int_num)
{
    IRR |= (0x1 << (int_num - master_base_vector));
}

uint8_t Interrupt_Controller_8259a::send_int_cpu()
{
    int i;
    for(i = 0; i < 8; i++)
    {
        if(0x1 << i & (IRR & (~IMR)))
        {
            ISR = 0x1 << i;
            IRR = IRR ^ (0x1 << i);
            return ICW[2] + i;
        }
    }
    return 0;

}
