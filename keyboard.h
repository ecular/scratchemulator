#ifndef EMU_KEYBOARD_H
#define EMU_KEYBOARD_H

#include <stdint.h>
#include <SDL/SDL.h>
#include "8259a.h"

class Keyboard {
private:
    uint8_t TranslateToScancode(uint16_t);
    SDL_Surface *screen_p;
    Interrupt_Controller_8259a *i8259a;
    uint8_t data;
public:
    void InitKeyboard(Interrupt_Controller_8259a *);
    void MonitorInput(void);
    uint8_t GetKey(void);
    void SetKey(void);
};

#endif
