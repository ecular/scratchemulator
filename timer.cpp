#include "timer.h"

#include <sys/time.h>
#include <stdio.h>

timer::timer(Interval_Timer_8253 &i8253_arg, Interrupt_Controller_8259a &i8259a_arg): i8253(i8253_arg), i8259a(i8259a_arg)
{
    irq0_timegap = 65536.0 * 1000000 / 1193181.6;
    i8253_tick_gap = 1000000.0 / 1193181.6;
}


void timer::timer_init()
{
    struct timeval tv;
    uint64_t current_time;
    gettimeofday(&tv, NULL);
    current_time = tv.tv_sec * 1000000 + tv.tv_usec;
    last_8253_count = last_irq0 = current_time;
}


inline void timer::timer_tick()
{
    struct timeval tv;
    uint64_t current_time;
    uint16_t counter_minus;
    gettimeofday(&tv, NULL);
    current_time = tv.tv_sec * 1000000 + tv.tv_usec;
    if(i8253.active[0] == 1) //irq0 is running
        if(current_time >= (last_irq0 + irq0_timegap))
        {
            last_irq0 = current_time;
            i8259a.trigger_int(0);//trigger 8259a irq0 interrupt
        }
    if(current_time >= (last_8253_count + i8253_tick_gap))
    {
        counter_minus = current_time / i8253_tick_gap;
        for(int i = 0; i < 3; ++i)
        {
            if(i8253.active[i] == i)
            {
                if(i8253.counter[i] - 1 < counter_minus)
                    i8253.counter[i] = (i8253.init_data_high[i] << 4) + i8253.init_data_low[i];
                i8253.counter[i] -= counter_minus;
            }
        }
        last_8253_count = current_time;
    }
}
