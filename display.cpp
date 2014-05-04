#include "display.h"
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>

#include "video.h"
#include "cpu.h"

Display::Display()
{
    cursor_visable = 0;
    pthread_mutex_init(&screen_lock, NULL);
}

void *Display::video_fresh(void *tmp)
{
    Display *p = (Display *)tmp;
    uint32_t cursor_pre_tick = SDL_GetTicks();
    uint32_t cursor_cur_tick;
    while(1)
    {
        /*cursor twinkle*/
        cursor_cur_tick = SDL_GetTicks();
        if(cursor_cur_tick - cursor_pre_tick >= 500)
        {
            cursor_pre_tick = cursor_cur_tick;
            p->cursor_visable = ~(p->cursor_visable) & 0x1;
            p->video->screen_update_flag = 1;
        }

        //if(p->video->screen_update_flag)
        {
            p->video->screen_update_flag = 0;
            if(p->screen_p != NULL)
            {
                pthread_mutex_lock(&(p->screen_lock));
                draw(tmp);
                pthread_mutex_unlock(&(p->screen_lock));
            }
        }
    }
    return NULL;
}

void Display::InitDisplay()
{
    pthread_t thread_id;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
    {
        std::cerr << "Could not initliaze SDL!" << std::endl;
        return;
    }
    screen_p = SDL_SetVideoMode(640, 400, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Emu", NULL);
    pthread_create(&thread_id, NULL, video_fresh, (void *)this);
}

void Display::draw(void *tmp)
{
    Display *p = (Display *)tmp;
    uint32_t vga_start_addr, char_addr, draw_pixel;
    uint8_t char_count_x, char_count_y;
    uint8_t char_current;
    uint8_t cursor_heigh = 2, cursor_width = 8;
    uint16_t cursor_pixel_x, cursor_pixel_y;
    switch(p->video->Video_Mode)
    {
    /*text mode*/
    case(0x0):
    case(0x1):
    case(0x2):
    case(0x3):
    case(0x7):
    {
        vga_start_addr = (p->video->CRT_Control_Reg[0xC] << 8) + p->video->CRT_Control_Reg[0xD];
        //printf("start render.\n");
        for(int y = 0; y < 400; ++y)
            for(int x = 0; x < 640 ; ++x)
            {
                char_count_y = y / 16; /*one char is 16 pixels height*/
                char_count_x = x / 8; /*one char is 8 pixels width*/
                char_addr = p->video->Video_Buffer_Address + (char_count_y * p->video->columns + char_count_x) * 2;
                char_current = p->cpu->ram[char_addr];
                draw_pixel = p->video->CGA_ascii_table[char_current * 128 + (y % 16) * 8 + (x % 8)]; /*a font in this table ,size is 8(x) * 16 (y). */
                if(p->video->Colorful_Flag)
                {
                    if(!draw_pixel)
                        draw_pixel = p->video->CGApalette[p->cpu->ram[char_addr + 1] >> 4]; //background
                    else
                        draw_pixel = p->video->CGApalette[p->cpu->ram[char_addr + 1] & 0xF]; //text color
                }
                else
                {
                    if(p->cpu->ram[char_addr + 1] & 0x70)
                        if(!draw_pixel)
                            draw_pixel = p->video->CGApalette[7];
                        else
                            draw_pixel = p->video->CGApalette[0];
                    else if(!draw_pixel)
                        draw_pixel = p->video->CGApalette[0];
                    else
                        draw_pixel = p->video->CGApalette[7];
                }
                p->display_buffer[y][x] = draw_pixel;
            }
        break;
    }
    }

    /*draw cursor*/
    if(p->video->Graphic_Mode_Flag == 0 && p->cursor_visable)
    {
        //printf("draw cursor!\n");
        cursor_pixel_x = p->video->cursor_x * cursor_width;
        //printf("cursor_x:%d,cursor_width:%d,cursor_pixel_x:%d\n",p->video->cursor_x,cursor_width,cursor_pixel_x);
        cursor_pixel_y = (p->video->cursor_y + 1) * 8 - cursor_heigh;
        for(int y = cursor_pixel_y * 2; y < cursor_pixel_y * 2  + cursor_heigh; ++y)
            for(int x = cursor_pixel_x; x < cursor_pixel_x + cursor_width; ++x)
            {
                draw_pixel = p->video->CGApalette[p->cpu->ram[p->video->Video_Buffer_Address + p->video->cursor_y * p->video->columns * 2 + p->video->cursor_x * 2 - 1] & 0xF];//cursor color
                p->display_buffer[y][x] = draw_pixel;
            }
    }
    /*prepare data (display_buffer[][]) finished,draw now*/
    SDL_Screen_Draw(tmp);
}

void Display::SDL_Screen_Draw(void *tmp)
{
    Display *p = (Display *)tmp;
    uint8_t red, green, blue;
    uint32_t offset_of_screen, pixelrgb;
    if(SDL_MUSTLOCK(p->screen_p))
        if(SDL_LockSurface(p->screen_p) < 0)
            return;
    for(int y = 0; y < p->screen_p->h; ++y)
    {
        offset_of_screen = y * p->screen_p->w;
        for(int x = 0; x < p->screen_p->w; ++x)
        {
            pixelrgb = p->display_buffer[y][x];
            //printf("pixelrgb %x\n",pixelrgb);
            blue = pixelrgb & 0xFF;
            green = (pixelrgb & 0xFF00) >> 8;
            red = (pixelrgb & 0xFF0000) >> 16;
            //printf("pix:%x red:%x green:%x blue:%x\n",pixelrgb,red,green,blue);
            ((uint32_t *)(p->screen_p->pixels))[offset_of_screen + x] = SDL_MapRGB(p->screen_p->format, red, green, blue);
            //((uint32_t *)(p->screen_p->pixels))[offset_of_screen + x] = SDL_MapRGB(p->screen_p->format, 255, 255, 255);
        }
    }
    if(SDL_MUSTLOCK(p->screen_p))
        SDL_UnlockSurface(p->screen_p);
    //printf("real draw.");
    SDL_UpdateRect(p->screen_p, 0, 0, p->screen_p->w, p->screen_p->h);
}

void Display::setcpu(Cpu *arg)
{
    cpu = arg;
}
void Display::setvideo(Video *arg)
{
    video = arg;
}


void Display::ChangeMode()
{
    //pthread_mutex_lock(&(p->screen_lock));
    if(video->Mode_Change_Flag)
    {
        //    if(screen_p != NULL)
        //        SDL_FreeSurface(screen_p);
    }
    //pthread_mutex_unlock(&(p->screen_lock));
}
