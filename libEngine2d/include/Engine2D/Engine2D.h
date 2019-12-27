#pragma once
#ifndef ENGINE_2D_H
#define ENGINE_2D_H

#include <engine2d/engine2d_types.h>
#include <engine2d/application2d.h>
#include "libEngine2d_image.h"

#ifdef USE_OOP
#ifndef APP_CLASS
    #error "When using POO you must define your main class which must derive from Engine2D::App2D"
#endif

extern "C"
{
    ENGINE2D_INITIALIZE_APPLICATION(Engine2D_InitializeApplication)
    {
        APP_CLASS *app = new APP_CLASS(state);
        state->userData = app;
    }

    ENGINE2D_APPLICATION_UPDATE(Engine2D_ApplicationUpdate)
    {
        APP_CLASS *app = (APP_CLASS*)state->userData;
        app->update(deltaTime, totalTime, state, input);
    }

    ENGINE2D_APPLICATION_UPDATE(Engine2D_QuitApplication)
    {
        APP_CLASS *app = (APP_CLASS*)state->userData;
        app->quit(state);
        delete app;
    }
}
#endif

#endif // ENGINE_2D_H
