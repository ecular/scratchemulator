#ifndef EMU_PORT_HANDLE_H
#define EMU_PORT_HANDLE_H

#include <stdint.h>

#include "8259a.h"
#include "8253.h"
#include "video.h"
#include "keyboard.h"

class port_handle {
private:
    friend class Cpu;
    Interrupt_Controller_8259a *i8259a;
    Interval_Timer_8253 *i8253;
    Video *video;
    Keyboard *keyboard;
    void port_handle_write8(uint16_t, uint8_t);
    uint8_t port_handle_read8(uint16_t);
    void port_handle_write16(uint8_t, uint16_t);
    uint16_t port_handle_read16(uint8_t);
public:
    port_handle();
    void seti8259a(Interrupt_Controller_8259a *);
    void seti8253(Interval_Timer_8253 *);
    void setvideo(Video *);
    void setkeyboard(Keyboard *);
};

#endif
