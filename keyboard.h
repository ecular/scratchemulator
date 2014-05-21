#ifndef EMU_KEYBOARD_H
#define EMU_KEYBOARD_H

#include <stdint.h>
#include <SDL/SDL.h>
#include "8259a.h"

class Keyboard {
private:
    SDL_Surface *screen_p;
    Interrupt_Controller_8259a *i8259a;
    uint8_t data;
public:
    void InitKeyboard(Interrupt_Controller_8259a *);
    void MonitorInput(void);
    uint8_t GetKey(void);
};

#endif
