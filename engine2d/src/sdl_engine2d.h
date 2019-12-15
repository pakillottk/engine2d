#pragma once
#ifndef ENGINE2D_SDL_H
#define ENGINE2D_SDL_H

#include "../include/engine2d/engine2d_types.h"
#include <SDL2/SDL.h>

struct SDLContext
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_PixelFormat *format;
    SDL_Texture *screen;
    Engine2D::ColorRGBA32 *framebuffer;
    u32 *screen_buffer;
    bool8 keypressed;
    real32 deltaTime;
    real32 totalTime;
};


#endif // ENGINE2D_SDL_H