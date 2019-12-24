#include "../include/engine2d/application2d.h"
#include <string.h>
using namespace Engine2D;

internal void initializeEngineState(EngineState *state)
{
    // def title
    strcpy(state->appTitle, "engine2d");
}