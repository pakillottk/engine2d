#include <SDL2/SDL.h>
#include "../include/engine2d/application2d.h"
#include "sdl_engine2d.h"
#include "engine2d_rect.cpp"
#include "engine2d_sprites.cpp"
#include "stb_engine2d_text.cpp"
#include "engine2d_layer.cpp"
using namespace Engine2D;

internal void mapRgbaToPixelFormat(SDL_PixelFormat *format, ColorRGBA32 *colors, u32 colorCount, u32 *buffer)
{
    for(u32 i = 0; i < colorCount; ++i)
    {
        buffer[i] = SDL_MapRGBA(format, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
    }
}

internal SDLContext makeSDLContext(EngineState *state, Size *screenSize)   
{
    SDLContext context;

    SDL_Init(SDL_INIT_VIDEO);

    context.window = SDL_CreateWindow(
        state->appTitle, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        screenSize->width, 
        screenSize->height, 
        SDL_WINDOW_OPENGL
    );
    context.renderer = SDL_CreateRenderer(context.window, 0, SDL_RENDERER_ACCELERATED);
    context.format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    context.screen = SDL_CreateTexture(
                            context.renderer, 
                            context.format->format, 
                            SDL_TEXTUREACCESS_STREAMING, 
                            screenSize->width, 
                            screenSize->height 
                        );
    context.framebuffer = (ColorRGBA32*)calloc(screenSize->width * screenSize->height, sizeof(ColorRGBA32));
    context.screen_buffer = (u32*)calloc(screenSize->width * screenSize->height, sizeof(u32));

    return(context); 
}

inline bool8 updateInput(SDLContext *context, EngineState *state, UserInput *input)
{
    SDL_Event event;
    while( SDL_PollEvent(&event) )
    {
        // TODO(pgm) handle well the events and map the user input
        switch (event.type)
        {                
            case SDL_QUIT:
                return(true);
            break;
            
            case SDL_KEYDOWN:
                context->keypressed =  true;
            break;

            case SDL_KEYUP:
                context->keypressed = false;
            break;

            default:
                break;
        }

        // TODO(pgm) handle well the events and map the user input
        SDL_Keycode key = event.key.keysym.sym;
        switch (key)
        {    
            case SDLK_LEFT:
                input->arrows.left = context->keypressed;
            break;
            case SDLK_UP:
                input->arrows.up = context->keypressed;
            break;
            case SDLK_RIGHT:
                input->arrows.right = context->keypressed;
            break;
            case SDLK_DOWN:
                input->arrows.down = context->keypressed;
            break;

            // TODO                
            default:
                break;
        }
    }

    return(false);
}

inline void render(SDLContext *context, EngineState *state, Size *screenSize)
{
    // TODO(pgm)
    // blend the layer pixels into the framebuffer
    memset(context->framebuffer, 0, sizeof(ColorRGBA32)*screenSize->width*screenSize->height);
    for(i32 i = 0; i < state->layerCount; ++i)
    {
        renderLayer( &state->layers[i], context->framebuffer, screenSize, &state->visibleRegion, state->tilemaps, state->tilemapCount );
    }
    // translate the framebuffer to the real u32 buffer
    mapRgbaToPixelFormat(context->format, context->framebuffer, screenSize->width * screenSize->height, context->screen_buffer);

    SDL_UpdateTexture(context->screen, NULL, context->screen_buffer, screenSize->width * sizeof(u32));
    SDL_RenderClear(context->renderer);
    SDL_RenderCopy(context->renderer, context->screen, NULL, NULL); 
    SDL_RenderPresent(context->renderer);

    // TODO(pgm) calculate delta time and so on
    SDL_Delay(16);            
}

internal void releaseSDLContext(SDLContext *context)
{
    SDL_DestroyWindow(context->window);
    SDL_DestroyTexture(context->screen);
    SDL_DestroyRenderer(context->renderer);
    SDL_FreeFormat(context->format);
    SDL_Quit();
    free(context->framebuffer);
    free(context->screen_buffer);
}