#ifndef EMU_DISPLAY_H
#define EMU_DISPLAY_H

#include <SDL/SDL.h>
#include <stdint.h>
#include "video.h"

class Cpu;

class Display {
private:
    SDL_Surface *screen_p;
    Cpu *cpu;
    Video *video;
    pthread_mutex_t screen_lock;
    uint8_t cursor_visable;
    uint32_t display_buffer[400][640];
    static void *video_fresh(void *);
    static void draw(void *);
    static void SDL_Screen_Draw(void *);

public:
    Display();
    void InitDisplay();
    void setcpu(Cpu *);
    void setvideo(Video *);
    void ChangeMode();
};

#endif
