#ifndef EMU_8259A_H_
#define EMU_8259A_H_

#include <stdint.h>

class Interrupt_Controller_8259a {
private:
    uint8_t IMR;
    uint8_t IRR;
    uint8_t ISR;
    uint8_t ICW[5];
    uint8_t OCW[4];
    int8_t init_order;
    uint8_t master_base_vector;
public:
    friend class Cpu;
    Interrupt_Controller_8259a(void);
    void init_8259a(void);
    uint8_t read_8259a(uint8_t port_num);
    void write_8259a(uint8_t, uint8_t);
    void trigger_int(uint8_t);
    uint8_t send_int_cpu(void);
};

#endif
