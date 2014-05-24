#ifndef EMU_TIMER_H
#define EMU_TIMER_H

#include <stdint.h>
#include <sys/time.h>
#include "8259a.h"
#include "8253.h"
#include "video.h"

class timer {
private:
    uint64_t last_8253_count, last_irq0, scan_gap, last_scan_tick, current_scan;
    Interval_Timer_8253 *i8253;
    Interrupt_Controller_8259a *i8259a;
    struct timeval tv;
    Video *video;
    double irq0_timegap, i8253_tick_gap;
public:
    timer();
    void timer_init();
    void timer_tick();
    void seti8253(Interval_Timer_8253 *);
    void seti8259a(Interrupt_Controller_8259a *);
    void setvideo(Video *);
};

#endif
