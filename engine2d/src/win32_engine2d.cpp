#include "../include/engine2d/application2d.h"
#include "engine2d.cpp"
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

#include "win32_engine2d_io.cpp"
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
    engine2d_quitApplication *quitApp;
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
        appCode.quitApp = (engine2d_quitApplication*)GetProcAddress(appCode.dllHandle, "Engine2D_QuitApplication");
        appCode.isValid = appCode.initializeApp != NULL && appCode.updateApp != NULL && appCode.quitApp != NULL;
    }

    if( !appCode.isValid )
    {
        appCode.initializeApp = initializeAppStub;
        appCode.updateApp  = applicationUpdateStub;
        appCode.quitApp = quitApplicationStub;
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
        app.quitApp = quitApplicationStub;
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

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow )
{
    LARGE_INTEGER perfCounterFreq;
    QueryPerformanceFrequency(&perfCounterFreq);

    AppCode appCode = loadApp(appDll);
    Size screenSize { SCREEN_WIDTH, SCREEN_HEIGHT };
    
    UserInput input = {};    
    EngineState state = {};
    initializeEngineState(&state);
    appCode.initializeApp(&state);
    
    // TODO(pgm) Handle window resizes
    real32 deltaTime = 0, totalTime = 0;
    SDLContext context = makeSDLContext(&state, &screenSize);
    Sprite **layerSpriteMap = (Sprite**)calloc(screenSize.width * screenSize.height, sizeof(Sprite*));
    bool8 exit = false;

    LARGE_INTEGER startCounter, endCounter;
    QueryPerformanceCounter(&startCounter);

    while( true )
    {       
        #if HOT_RELOAD
            if( attemptHotReload(appDll, appCode) )
            {
                appCode.initializeApp(&state);
            }
        #endif

        // check for user inputs
        exit = updateInput(&context, &screenSize, &state, &input);
        if( exit )
        {
            break;
        }
        // update the state
        appCode.updateApp(deltaTime, totalTime, &state, &input);

        // render the new state
        render(&context, &state, &screenSize);

        QueryPerformanceCounter(&endCounter);

        u64 timeElapsed = endCounter.QuadPart - startCounter.QuadPart;
        deltaTime = real64(timeElapsed) / real64(perfCounterFreq.QuadPart);
        totalTime += deltaTime;

        // u32 frameMs = (u32)((timeElapsed * 1000) / perfCounterFreq.QuadPart);
        // u32 fps = perfCounterFreq.QuadPart / timeElapsed;

        // char buffer[512];
        // wsprintf(buffer, "ms/frame: %d ms / FPS: %d\n", frameMs, fps);
        // OutputDebugStringA(buffer);

        startCounter = endCounter;
    }
    
    unloadApp(appCode);
    #if HOT_RELOAD
        DeleteFileA(liveDll);
    #endif

    releaseSDLContext(&context);
    for( u32 i = 0; i < state.layerCount; ++i )
    {
        releaseLayer( &state.layers[i] );
    }
    free(layerSpriteMap);
    return(0);
}