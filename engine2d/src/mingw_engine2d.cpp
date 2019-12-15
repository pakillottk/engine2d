#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#ifndef NULL
    #define NULL 0
#endif
#ifndef FALSE
    #define FALSE 0
#endif
#ifndef TRUE
    #define TRUE 1
#endif

#include "../include/engine2d/application2d.h"
#include "sdl_engine2d.cpp"

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

    SDLContext context = makeSDLContext(&state, &screenSize);
    bool8 exit = false;
    while( true )
    {
        #if HOT_RELOAD
            attemptHotReload(appDll, appCode);
        #endif

        exit = updateInput(&context, &state, &input);
        if( exit )
        {
            break;
        }
        appCode.updateApp(context.deltaTime, context.totalTime, &state, &input);
        render(&context, &state, &screenSize);
    }
    
    for( u32 i = 0; i < state.layerCount; ++i )
    {
        releaseLayer( &state.layers[i] );
    }
    releaseSDLContext(&context);
    return(0);
}