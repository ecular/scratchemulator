#include "keyboard.h"
#include <unistd.h>

void Keyboard::InitKeyboard(Interrupt_Controller_8259a *i8259a_arg)
{
    i8259a = i8259a_arg;
}

void Keyboard::MonitorInput(void)
{
    SDL_Event event;
    while(1)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == 0x20)
                {
                    data = 0x39;
                    i8259a->trigger_int(1);
                }
            }
            if(event.type == SDL_QUIT)
                return;
        }
        usleep(100);
    }
}

inline uint8_t Keyboard::GetKey()
{
    return data;
}
