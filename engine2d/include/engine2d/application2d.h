#pragma once
#ifndef APPLICATION_2D_H
#define APPLICATION_2D_H

#include "engine2d_types.h"
#include "engine2d_state.h"

namespace Engine2D
{   
    struct UserInput
    {
        // TODO(pgm) This is a dummy input for initial experimentation
        union 
        {
            byte pressed;
            struct
            {
                byte left : 1;
                byte up : 1;
                byte right : 1;
                byte down : 1;
                byte zoomPlus : 1;
                byte zoomMinus : 1;                
            };
        } buttons;
    };
}

// TODO(pgm) This shouldn't live here?
#define ENGINE2D_INITIALIZE_APPLICATION(name) void name(Engine2D::EngineState *state)
typedef ENGINE2D_INITIALIZE_APPLICATION(engine2d_initializeApplication);
ENGINE2D_INITIALIZE_APPLICATION(initializeAppStub)
{}

// TODO(pgm) This shouldn't live here?
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