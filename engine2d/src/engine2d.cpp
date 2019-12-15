#include "../include/engine2d/application2d.h"
#include <string.h>
using namespace Engine2D;

internal void initializeEngineState(EngineState *state)
{
    strcpy(state->appTitle, "engine2d");
    state->layerCount = 0;
    for( u32 i = 0; i < MAX_LAYER_COUNT; ++i )
    {
        state->layers[i].attributes.flags = 0;
        state->layers[i].spriteCount = 0;
        state->layers[i].textsCount = 0;
        for( u32 j = 0; j < MAX_SPRITE_COUNT; ++j )
        {
            state->layers[j].sprites[j].size = { 0, 0 };
            state->layers[j].sprites[j].worldPosition = { 0, 0 };
        }
    }

}