#ifndef EMU_TIMER_H
#define EMU_TIMER_H

#include <stdint.h>
#include "8259a.h"
#include "8253.h"

class timer {
private:
    uint64_t last_8253_count, last_irq0;
    Interval_Timer_8253 &i8253;
    Interrupt_Controller_8259a &i8259a;
    double irq0_timegap, i8253_tick_gap;
public:
    timer(Interval_Timer_8253 &, Interrupt_Controller_8259a &);
    void timer_init();
    void timer_tick();
};

#endif
