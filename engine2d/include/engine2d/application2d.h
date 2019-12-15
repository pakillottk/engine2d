#pragma once
#ifndef APPLICATION_2D_H
#define APPLICATION_2D_H

#include "engine2d_types.h"
#include "engine2d_layer.h"

#define MAX_APP_TITLE_LENGTH 1024
#define MAX_LAYER_COUNT 128

namespace Engine2D
{
    struct EngineState 
    {
        char appTitle[MAX_APP_TITLE_LENGTH];
        // TODO(pgm)
        // Size screenSize; 
        ScreenRect visibleRegion;
        Layer layers[MAX_LAYER_COUNT];
        u32 layerCount;
    };

    struct UserInput
    {
        // TODO(pgm) This is a dummy input for initial experimentation
        union 
        {
            byte directions;
            struct
            {
                byte left : 1;
                byte up : 1;
                byte right : 1;
                byte down : 1;
                // unused but we can't make less than 1 byte
                byte padding : 4;
            };
        } arrows;
    };
}

#define ENGINE2D_INITIALIZE_APPLICATION(name) void name(Engine2D::EngineState *state)
typedef ENGINE2D_INITIALIZE_APPLICATION(engine2d_initializeApplication);
ENGINE2D_INITIALIZE_APPLICATION(initializeAppStub)
{}

#define ENGINE2D_APPLICATION_UPDATE(name) void name(real32 deltaTime, real32 totalTime, Engine2D::EngineState *state, Engine2D::UserInput *input)
typedef ENGINE2D_APPLICATION_UPDATE(engine2d_applicationUpdate);
ENGINE2D_APPLICATION_UPDATE(applicationUpdateStub)
{}

// Code that the engine calls
#ifdef __cplusplus
extern "C" {
#endif

ENGINE2D_INITIALIZE_APPLICATION(Engine2D_InitializeApplication);
ENGINE2D_APPLICATION_UPDATE(Engine2D_ApplicationUpdate);

#ifdef __cplusplus
}
#endif

#endif // APPLICATION_2D_H