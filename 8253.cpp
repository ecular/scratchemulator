#include "8253.h"

Interval_Timer_8253::Interval_Timer_8253()
{
    int i;
    for(i = 0; i < 3; i++)
        rw_step[i] = 0;
}

uint8_t Interval_Timer_8253::read_8253(uint8_t port_num)
{
    uint8_t selected_channel = port_num & 0x03;
    uint8_t tmp_data;
    switch(selected_channel)
    {
    case(0x00):/*channel 0*/
    case(0x01):/*channel 1*/
    case(0x02):/*channel 2*/
    {
        switch(rw_mode[selected_channel])
        {
        case(0x01):/*low 8bit*/
        {
            tmp_data = *locked_data_pointer[selected_channel];
            locked_data_pointer[selected_channel] = &counter[selected_channel];
            return tmp_data & 0x00FF;
        }
        case(0x02):/*high 8bit*/
        {
            tmp_data = *locked_data_pointer[selected_channel];
            locked_data_pointer[selected_channel] = &counter[selected_channel];
            return tmp_data >> 8;
        }
        case(0x03):/*low and high*/
        {
            tmp_data = *locked_data_pointer[selected_channel];
            if(rw_step[selected_channel])/*high 8bit*/
            {
                rw_step[selected_channel] = 0;
                locked_data_pointer[selected_channel] = &counter[selected_channel];
                return tmp_data >> 8;
            }
            else/*low 8bit*/
            {
                rw_step[selected_channel] = 1;
                return tmp_data & 0x00FF;
            }
        }
        default:
            return 0;
        }
    }
    }
    return 0;
}
void Interval_Timer_8253::write_8253(uint8_t port_num, uint8_t value)
{
    switch(port_num)
    {
    case(0x40):/*channel 0*/
    case(0x41):/*channel 1*/
    case(0x42):/*channel 2*/
    {
        uint8_t channel_tmp = port_num & 0x03;
        switch(rw_mode[channel_tmp])
        {
        case(0x01):/*low 8bit*/
        {
            init_data_low[channel_tmp] = value;
            init_data_high[channel_tmp] = 0;
            break;
        }
        case(0x02):/*high 8bit*/
        {
            init_data_high[channel_tmp] = value;
            init_data_low[channel_tmp] = 0;
            break;
        }
        case(0x03):/*low and high*/
        {
            if(rw_step[channel_tmp])
            {
                init_data_high[channel_tmp] = value;
                rw_step[channel_tmp] = 0;
            }
            else
            {
                init_data_low[channel_tmp] = value;
                rw_step[channel_tmp] = 1;
            }
            break;
        }
        default:
            break;
        }
    }

    case(0x43):/*mode control*/
    {
        uint8_t selected_channel;
        switch(value >> 6)/*channel selector*/
        {
        case(0x00):/*channel 0*/
        {
            selected_channel = 0;
            break;
        }
        case(0x01):/*channel 1*/
        {
            selected_channel = 1;
            break;
        }
        case(0x02):/*channel 2*/
        {
            selected_channel = 2;
            break;
        }
        default:
            return;
        }
        switch((value >> 4) & 0x03) /*R/W mode selector*/
        {
        case(0x00):/*data lock*/
        {
            locked_data[selected_channel] = counter[selected_channel];
            locked_data_pointer[selected_channel] = &locked_data[selected_channel];
            break;
        }
        case(0x01):/*low 8bit*/
        {
            rw_mode[selected_channel] = 0x1;
            break;
        }
        case(0x02):/*high 8bit*/
        {
            rw_mode[selected_channel] = 0x2;
            break;
        }
        case(0x03):/*low and high*/
        {
            rw_mode[selected_channel] = 0x3;
            break;
        }
        }
        break;
    }
    }
}
