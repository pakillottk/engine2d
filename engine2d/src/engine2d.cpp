#include "../include/engine2d/application2d.h"
#include <string.h>
using namespace Engine2D;

internal ENGINE2D_ALLOC(Engine2D_Alloc)
{
    // use calloc to initialize at zero
    return calloc(size, 1);
}

internal ENGINE2D_FREE(Engine2D_Free)
{
    free(memory);
}

internal void initializeEngineState(EngineState *state)
{
    // def title
    strcpy(state->appTitle, "engine2d");
    // register callbacks
    state->malloc = Engine2D_Alloc;
    state->free = Engine2D_Free;
}