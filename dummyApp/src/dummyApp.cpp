#include <stdio.h>
#include <string.h>
#include <engine2d/application2d.h>

void Engine2D_InitializeApplication(EngineState *state)
{
    strcpy(state->appTitle, "DummyApp"); 
}

void Engine2D_InputUpdated(EngineState *state, UserInput *input)
{
    state->clearColor = 0xffff00;
}