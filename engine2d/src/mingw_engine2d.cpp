#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <SDL2/SDL.h>

#include "../include/engine2d/application2d.h"
#include "engine2d_rect.cpp"
#include "engine2d_sprites.cpp"
using namespace Engine2D;

#ifndef NULL
    #define NULL 0
#endif
#ifndef FALSE
    #define FALSE 0
#endif
#ifndef TRUE
    #define TRUE 1
#endif
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define internal static
#define keep_alive_var static

internal FILETIME Win32_GetLastWriteTime(const char *filename)
{
    FILETIME lastWriteTime = {};

    WIN32_FIND_DATA findData;
    HANDLE fHandle = FindFirstFileA(filename, &findData);
    if( fHandle != INVALID_HANDLE_VALUE )
    {
        lastWriteTime = findData.ftLastWriteTime;
        FindClose(fHandle);
    }
    return(lastWriteTime);
}

struct AppCode
{
    bool8 isValid;
    HMODULE dllHandle;
    FILETIME lastWriteTime;
    engine2d_initializeApplication *initializeApp;
    engine2d_applicationUpdate *updateApp;
};

// used for hot reload
keep_alive_var const char *liveDll = "AppLive.dll";

internal AppCode loadApp(const char *targetFilename)
{
    AppCode appCode;
    appCode.isValid = 0;
    appCode.lastWriteTime = Win32_GetLastWriteTime(targetFilename);

    #if HOT_RELOAD
        CopyFileA(targetFilename, liveDll, false);
        appCode.dllHandle = LoadLibraryA(liveDll);
    #else
        appCode.dllHandle = LoadLibraryA(targetFilename);
    #endif

    if( appCode.dllHandle )
    {
        appCode.initializeApp = (engine2d_initializeApplication*)GetProcAddress(appCode.dllHandle, "Engine2D_InitializeApplication");
        appCode.updateApp = (engine2d_applicationUpdate*)GetProcAddress(appCode.dllHandle, "Engine2D_ApplicationUpdate");
        appCode.isValid = appCode.initializeApp != NULL && appCode.updateApp != NULL;
    }

    if( !appCode.isValid )
    {
        appCode.initializeApp = initializeAppStub;
        appCode.updateApp  = applicationUpdateStub;
    }

    return appCode;
} 

internal void unloadApp(AppCode &app)
{
    if( app.isValid )
    {
        app.isValid = 0;
        app.initializeApp = initializeAppStub;
        app.updateApp  = applicationUpdateStub;
        FreeLibrary( app.dllHandle );
    }
}


#if HOT_RELOAD
internal bool8 attemptHotReload(const char * targetFilename, AppCode &app)
{
    FILETIME targetWriteTime = Win32_GetLastWriteTime(targetFilename);
    if( CompareFileTime(&targetWriteTime, &app.lastWriteTime) != 0 )
    {
        // reload
        unloadApp(app);
        app = loadApp(targetFilename);

        return(TRUE);
    }

    return(FALSE);
}
#endif

internal u32* mapRgbaToPixelFormat(SDL_PixelFormat *format, ColorRGBA32 *colors, u32 colorCount)
{
    u32 *mappedColors = (u32*)malloc(colorCount * sizeof(u32));
    for(u32 i = 0; i < colorCount; ++i)
    {
        mappedColors[i] = SDL_MapRGBA(format, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
    }

    return mappedColors;
}

// TODO(pgm): For now is hard coded. Could come in args
keep_alive_var const char *appDll = "App.dll";

// NOTE(pgm): This is just a sort of a TestApp. In the end all that will happen it's that we'll have these things called by an external
// app
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow )
{
    AppCode appCode = loadApp(appDll);
    Size screenSize { SCREEN_WIDTH, SCREEN_HEIGHT };
    UserInput input;
    input.arrows.directions = 0;
    EngineState state;
    strcpy(state.appTitle, "engine2d");
    appCode.initializeApp(&state);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *wnd = SDL_CreateWindow(
        state.appTitle, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        screenSize.width, 
        screenSize.height, 
        SDL_WINDOW_OPENGL
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(wnd, 0, SDL_RENDERER_SOFTWARE);
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    SDL_Texture *screen = SDL_CreateTexture(
                            renderer, 
                            format->format, 
                            SDL_TEXTUREACCESS_STREAMING, 
                            screenSize.width, 
                            screenSize.height 
                        );

    u32 *screen_buffer = (u32*)calloc(screenSize.width * screenSize.height, sizeof(u32));
    bool8 exit = false, keypressed = false;
    while( !exit )
    {
        #if HOT_RELOAD
            attemptHotReload(appDll, appCode);
        #endif

        SDL_Event event;
        while( SDL_PollEvent(&event) )
        {
            // TODO(pgm) handle well the events and map the user input
            switch (event.type)
            {                
                case SDL_QUIT:
                    exit = true;
                    break;
                
                case SDL_KEYDOWN:
                    keypressed =  true;
                break;

                case SDL_KEYUP:
                    keypressed = false;
                break;

                default:
                    break;
            }

            // TODO(pgm) handle well the events and map the user input
            SDL_Keycode key = event.key.keysym.sym;
            switch (key)
            {    
                case SDLK_LEFT:
                    input.arrows.left = keypressed;
                break;
                case SDLK_UP:
                    input.arrows.up = keypressed;
                break;
                case SDLK_RIGHT:
                    input.arrows.right = keypressed;
                break;
                case SDLK_DOWN:
                    input.arrows.down = keypressed;
                break;

                // TODO                
                default:
                    break;
            }
        }

        // TODO(pgm) pass the real timers
        appCode.updateApp(0, 0, &state, &input);

        // TODO(pgm): for now we just use one layer
        ScreenRect viewport { 0, 0, screenSize.width, screenSize.height };
        if( state.layers[0].attributes.patternBackground )
        {
            u32 *colors = mapRgbaToPixelFormat(
                            format, 
                            state.layers[0].backgroundPattern,
                            state.layers[0].patternSize.width * state.layers[0].patternSize.height
                        );
            drawScreenRectPattern( 
                viewport, 
                screenSize, 
                colors,  
                state.layers[0].patternSize,
                screen_buffer
            );
            free(colors);
        }
        else
        {
            u32 bgColor = SDL_MapRGBA(
                            format,  
                            state.layers[0].backgroundColor.r, 
                            state.layers[0].backgroundColor.g, 
                            state.layers[0].backgroundColor.b, 
                            state.layers[0].backgroundColor.a
                        );
            drawScreenRect( viewport, screenSize, bgColor, screen_buffer );
        }
        

        for(u32 i = 0; i < state.layers[0].spriteCount; ++i)
        {
            Sprite *sprite = &state.layers[0].sprites[i];
            ScreenRect rect = spriteScreenRect( sprite );
            ScreenRect projectedRect = mapScreenRectToViewport(  rect, screenSize, state.visibleRegion );
            u32 *colors = mapRgbaToPixelFormat(format, sprite->pixels, sprite->size.width * sprite->size.height);
            drawScreenRect(projectedRect, screenSize, colors, screen_buffer );
            free(colors);
        }

        SDL_UpdateTexture(screen, NULL, screen_buffer, screenSize.width * sizeof(u32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, screen, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);            
    }
    
    // TODO(pgm) release layers memory

    SDL_DestroyWindow(wnd);
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(renderer);
    SDL_FreeFormat(format);
    free(screen_buffer);
    return(0);
}