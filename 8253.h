#ifndef EMU_8253_H
#define EMU_8253_H

#include <stdint.h>

class Interval_Timer_8253 {
private:
    friend class timer;
    uint16_t counter[3];
    uint8_t init_data_high[3], init_data_low[3];
    uint16_t locked_data[3], *locked_data_pointer[3];
    uint8_t rw_mode[3], rw_step[3], active[3];
public:
    Interval_Timer_8253();
    uint8_t read_8253(uint8_t);
    void write_8253(uint8_t port_num, uint8_t value);
};

#endif
