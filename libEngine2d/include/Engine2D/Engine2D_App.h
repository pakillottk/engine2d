#pragma once
#ifndef ENGINE2D_APP_H
#define ENGINE2D_APP_H
#include <engine2d/application2d.h>

namespace Engine2D
{
    class App2D
    {
    public:
        App2D(Engine2D::EngineState *state)
        {}
        ~App2D()
        {}
        
        virtual void update(real32 deltaTime, real32 totalTime, Engine2D::EngineState *state, Engine2D::UserInput *input) = 0;
        virtual void quit(Engine2D::EngineState *state)
        {}
    };
}

#endif // ENGINE2D_APP_H