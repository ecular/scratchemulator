#include "timer.h"
#include <stdio.h>
#include <unistd.h>

timer::timer()
{
    irq0_timegap = 65536.0 * 1000000 / 1193181.6;
    i8253_tick_gap = 1000000.0 / 1193181.6;
    scan_gap = 1000000 / 31500;
    last_scan_tick = current_scan = 0;
}

void timer::seti8253(Interval_Timer_8253 *arg)
{
    i8253 = arg;
}

void timer::seti8259a(Interrupt_Controller_8259a *arg)
{
    i8259a = arg;
}

void timer::setvideo(Video *arg)
{
    video = arg;
}

void timer::timer_init()
{
    struct timeval tv;
    uint64_t current_time;
    gettimeofday(&tv, NULL);
    current_time = tv.tv_sec * 1000000 + tv.tv_usec;
    last_scan_tick = last_8253_count = last_irq0 = current_time;
}


inline void timer::timer_tick()
{
    uint64_t current_time;
    uint16_t counter_minus;
    //gettimeofday(&tv, NULL);
    //current_time = tv.tv_sec * 1000000 + tv.tv_usec;
    current_time = last_scan_tick + scan_gap + 10;
    if(current_time >= (last_scan_tick + scan_gap))
    {
        current_scan = (current_scan + 1) % 525;
        if(current_scan >= 480)
            video->Input_Status_Reg1 = 0x8;// vertical retrace
        else
            video->Input_Status_Reg1 = 0;

        if(current_scan & 0x01)// horizontal scan
            video->Input_Status_Reg1 |= 0x1;
        last_scan_tick = current_time;
    }

    if(i8253->active[0]) //irq0 is running
        if(current_time >= (last_irq0 + irq0_timegap))
        {
            last_irq0 = current_time;
            i8259a->trigger_int(0);//trigger 8259a irq0 interrupt
        }

    if(current_time >= (last_8253_count + i8253_tick_gap))
    {
        counter_minus = current_time / i8253_tick_gap;
        for(int i = 0; i < 3; ++i)
        {
            if(i8253->active[i] == 1)
            {
                if(i8253->counter[i] - 1 < counter_minus)
                    i8253->counter[i] = (i8253->init_data_high[i] << 4) + i8253->init_data_low[i];
                i8253->counter[i] -= counter_minus;
            }
        }
        last_8253_count = current_time;
    }
}
