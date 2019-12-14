#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <SDL2/SDL.h>

#include "../include/engine2d/application2d.h"

#ifndef NULL
    #define NULL 0
#endif
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define internal static

#define NORMALIZE_TO_RANGE(value, min, max) ( (value) - (min) ) / ( (max) - (min) )

#define ENGINE2D_INITIALIZE_APPLICATION(name) void name(EngineState*)
typedef ENGINE2D_INITIALIZE_APPLICATION(engine2d_initializeApplication);
ENGINE2D_INITIALIZE_APPLICATION(initializeAppStub)
{}

#define ENGINE2D_INPUT_UPDATED(name) void name(EngineState*, UserInput*)
typedef ENGINE2D_INPUT_UPDATED(engine2d_inputUpdated);
ENGINE2D_INPUT_UPDATED(inputUpdatedStub)
{}

struct AppCode
{
    bool8 isValid;
    HMODULE dllHandle;
    engine2d_initializeApplication *initializeApp;
    engine2d_inputUpdated *inputUpdated;
};

internal AppCode loadApp(void)
{
    AppCode appCode;
    appCode.isValid = 0;

    appCode.dllHandle = LoadLibraryA("App.dll");
    if( appCode.dllHandle )
    {
        appCode.initializeApp = (engine2d_initializeApplication*)GetProcAddress(appCode.dllHandle, "Engine2D_InitializeApplication");
        appCode.inputUpdated = (engine2d_inputUpdated*)GetProcAddress(appCode.dllHandle, "Engine2D_InputUpdated");
        appCode.isValid = appCode.initializeApp != NULL && appCode.inputUpdated != NULL;
    }

    if( !appCode.isValid )
    {
        appCode.initializeApp = initializeAppStub;
        appCode.inputUpdated  = inputUpdatedStub;
    }

    return appCode;
} 

internal void unloadApp(AppCode &app)
{
    if( app.isValid )
    {
        app.isValid = 0;
        app.initializeApp = initializeAppStub;
        app.inputUpdated  = inputUpdatedStub;
        FreeLibrary( app.dllHandle );
    }
}

struct ScreenRect
{
    unsigned x;
    unsigned y;
    unsigned width;
    unsigned height;
};

internal inline real32 screenRectAspectRatio(const ScreenRect &rect)
{
    return real32(rect.width) / real32(rect.height);
}

internal void drawScreenRect(const ScreenRect &rect, u32 color, u32 *screen)
{
    int x, y;
    for(unsigned row = 0; row < rect.width; ++row)
    {
        for(unsigned col = 0; col < rect.height; ++col)
        {
            x = rect.x + col;
            y = row + rect.y;
            // TODO(pgm): This can be optimized to get first the subrect that is in bounds
            if( x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT )
            {
                // this pixel it's out of bounds
                continue;
            }
            screen[ (y * SCREEN_WIDTH) + x ] = color;
        }
    }
}

struct Rect
{
    real32 x;
    real32 y;
    real32 width;
    real32 height;
};

internal inline real32 rectAspectRatio(const Rect &rect)
{
    return rect.width / rect.height;
}

internal void scaleRect(Rect &rect, real32 scale)
{
    real32 centerX = rect.x + rect.width  * 0.5f;
    real32 centerY = rect.y + rect.height * 0.5f;

    real32 targetWidth  = rect.width * scale;
    real32 targetHeight = rect.height * scale;

    rect.x = centerX - targetWidth * 0.5f;
    rect.y = centerY - targetHeight * 0.5f;
    rect.width = targetWidth;
    rect.height = targetHeight;
}

internal ScreenRect mapRectToScreen(const Rect &rect, const Rect &gameRect)
{
    ScreenRect result;
    result.width = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.width) * SCREEN_WIDTH;
    result.height = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.height) * SCREEN_HEIGHT;
    result.x = NORMALIZE_TO_RANGE(rect.x, gameRect.x, gameRect.x + gameRect.width) * SCREEN_WIDTH;
    result.y = SCREEN_HEIGHT - result.height - NORMALIZE_TO_RANGE(rect.y, gameRect.y, gameRect.y + gameRect.height) * SCREEN_HEIGHT;

    return(result);
}

// NOTE(pgm): This is just a sort of a TestApp. In the end all that will happen it's that we'll have these things called by an external
// app
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow )
{
    AppCode appCode = loadApp();
    UserInput input;

    EngineState state;
    strcpy(state.appTitle, "engine2d");
    state.clearColor = 0;
    appCode.initializeApp(&state);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *wnd = SDL_CreateWindow(
        state.appTitle, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_OPENGL
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(wnd, 0, SDL_RENDERER_SOFTWARE);
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    SDL_Texture *screen = SDL_CreateTexture(
                            renderer, 
                            format->format, 
                            SDL_TEXTUREACCESS_STREAMING, 
                            SCREEN_WIDTH, 
                            SCREEN_HEIGHT 
                        );

    u32 *screen_buffer = (u32*)calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(u32));
    Rect gameRect { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    Rect rect { 395, 295, 10, 10 };
    ScreenRect screenMappedRect;

    bool8 exit = false, keypressed = false, keyreleased = false;
    while( !exit )
    {
        SDL_Event event;
        while( SDL_PollEvent(&event) )
        {
            switch (event.type)
            {                
                case SDL_QUIT:
                    exit = true;
                    break;
                
                case SDL_KEYDOWN:
                    keypressed =  true;
                break;

                case SDL_KEYUP:
                    keyreleased = true;
                break;

                default:
                    break;
            }

            SDL_Keycode key = event.key.keysym.sym;
            if( keypressed )
            {
                switch (key)
                {
                    case SDLK_u:
                        unloadApp( appCode );
                    break;

                    case SDLK_r:
                        unloadApp( appCode );
                        appCode = loadApp();
                    break;

                    case SDLK_KP_PLUS:
                        scaleRect(gameRect, 0.9f);
                    break;

                    case SDLK_KP_MINUS:
                        scaleRect(gameRect, 1.1f);
                    break;

                    case SDLK_LEFT:
                        rect.x -= 1;
                    break;

                    case SDLK_RIGHT:
                        rect.x += 1;
                    break;

                    case SDLK_UP:
                        rect.y += 1;
                    break;

                    case SDLK_DOWN:
                        rect.y -= 1;
                    break;
                    
                    default:
                        break;
                }

                appCode.inputUpdated(&state, &input);
            }

            memset(screen_buffer, state.clearColor, sizeof(u32) * SCREEN_WIDTH * SCREEN_HEIGHT);

            screenMappedRect = mapRectToScreen(rect, gameRect);
            drawScreenRect( screenMappedRect, SDL_MapRGB(format, 255, 0, 0), screen_buffer );

            SDL_UpdateTexture(screen, NULL, screen_buffer, SCREEN_WIDTH * sizeof(u32));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, screen, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

        SDL_Delay(1);            
    }
    
    SDL_DestroyWindow(wnd);
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(renderer);
    SDL_FreeFormat(format);
    free(screen_buffer);
    return(0);
}