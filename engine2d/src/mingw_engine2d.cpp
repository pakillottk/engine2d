#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <SDL2/SDL.h>

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow )
{
    SDL_Window *wnd;
    SDL_Init(SDL_INIT_VIDEO);

    wnd = SDL_CreateWindow(
        "engine2d", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        800, 
        600, 
        SDL_WINDOW_OPENGL)
    ;
    
    SDL_Delay(3000);
    SDL_DestroyWindow(wnd);

    return(0);
}