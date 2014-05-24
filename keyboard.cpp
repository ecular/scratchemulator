#include "keyboard.h"
#include <unistd.h>

uint8_t Keyboard::TranslateToScancode(uint16_t SDL_code)
{
    switch(SDL_code)
    {
    case(0x1B)://ESC
        return (0x1);

    /*F1---F10*/
    case(0x11A):
    case(0x11B):
    case(0x11C):
    case(0x11D):
    case(0x11E):
    case(0x11F):
    case(0x120):
    case(0x121):
    case(0x122):
    case(0x123):
        return (SDL_code - 0xDF);

    case(0x124)://F11
        return (0x57);

    case(0x125)://F12
        return (0x58);

    case(0x116)://home
        return (0xC7);

    case(0x117)://end
        return (0xCF);

    case(0x115)://insert
        return (0xD2);

    case(0x7F)://del
        return (0xD3);

    case(0x60)://`
        return (0x29);

    case(0x30)://0
        return (0xB);

    /*1----9*/
    case(0x31):
    case(0x32):
    case(0x33):
    case(0x34):
    case(0x35):
    case(0x36):
    case(0x37):
    case(0x38):
    case(0x39):
        return (SDL_code - 0x2F);

    case(0x2D)://-
        return (0xC);

    case(0x3D)://=
        return (0xD);

    case(0x8)://back
        return (0xE);

    case(0x9)://Tab
        return (0xF);

    /*a--z*/
    case(0x61)://a
        return (0x1E);
    case(0x62)://b
        return (0x30);
    case(0x63)://c
        return (0x2E);
    case(0x64)://d
        return (0x20);
    case(0x65)://e
        return (0x12);
    case(0x66)://f
        return (0x21);
    case(0x67)://g
        return (0x22);
    case(0x68)://h
        return (0x23);
    case(0x69)://i
        return (0x17);
    case(0x6A)://j
        return (0x24);
    case(0x6B)://k
        return (0x25);
    case(0x6C)://l
        return (0x26);
    case(0x6D)://m
        return (0x32);
    case(0x6E)://n
        return (0x31);
    case(0x6F)://o
        return (0x18);
    case(0x70)://p
        return (0x19);
    case(0x71)://q
        return (0x10);
    case(0x72)://r
        return (0x13);
    case(0x73)://s
        return (0x1F);
    case(0x74)://t
        return (0x14);
    case(0x75)://u
        return (0x16);
    case(0x76)://v
        return (0x2F);
    case(0x77)://w
        return (0x11);
    case(0x78)://x
        return (0x2D);
    case(0x79)://y
        return (0x15);
    case(0x7A)://z
        return (0x2C);

    case(0x5B)://[
        return (0x1A);

    case(0x5D)://]
        return (0x1B);

    case(0x5C)://|
        return (0x2B);

    case(0x12D)://caps
        return (0x3A);

    case(0x3B)://;
        return (0x27);

    case(0x27)://'
        return (0x28);

    case(0xD)://enter
        return (0x1C);

    case(0x130)://Lshift
        return (0x2A);

    case(0x2C)://,
        return (0x33);

    case(0x2E)://.
        return (0x34);

    case(0x2F):///
        return (0x35);

    case(0x12F)://RShift
        return (0x36);

    case(0x132)://Lctrl
        return (0x1D);

    case(0x137)://Win
        return (0xDB);

    case(0x134)://Lalt
        return (0xB8);

    case(0x20)://space
        return (0x39);

    case(0x133)://Ralt
        return (0xB8);

    case(0x131)://Rctrl
        return (0x9D);

    case(0x111)://up
        return (0x48);

    case(0x112)://down
        return (0x50);

    case(0x114)://left
        return (0x4B);

    case(0x113)://right
        return (0x4D);

    case(0x118)://pageup
        return (0xC9);

    case(0x119)://pagedown
        return (0xD1);
    default:
        return 0;
    }
    return 0;
}

void Keyboard::InitKeyboard(Interrupt_Controller_8259a *i8259a_arg)
{
    i8259a = i8259a_arg;
}

void Keyboard::MonitorInput(void)
{
    SDL_Event event;
    while(1)
    {
        if(SDL_WaitEvent(&event))
        {
            switch(event.type)
            {
            case(SDL_KEYDOWN):
            {
                data = TranslateToScancode(event.key.keysym.sym);
                i8259a->trigger_int(1);
                break;
            }
            case(SDL_KEYUP):
            {
                data = TranslateToScancode(event.key.keysym.sym) | 0x80;
                i8259a->trigger_int(1);
                break;
            }
            case(SDL_QUIT):
            {
                return;
            }
            }
        }
        usleep(100);
    }
}

inline uint8_t Keyboard::GetKey()
{
    return data;
}

inline void Keyboard::SetKey()
{
    data  = 0;
}
