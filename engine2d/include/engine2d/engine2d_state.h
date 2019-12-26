#pragma once
#ifndef ENGINE2D_STATE_H
#define ENGINE2D_STATE_H

#include "engine2d_types.h"
#include "engine2d_layer.h"
#include "engine2d_tilemap.h"

#define MAX_APP_TITLE_LENGTH 1024
#define MAX_LAYER_COUNT 128

// TODO(pgm) This shouldn't live here?
#define ENGINE2D_ALLOC(name) void* name(size_t size)
typedef ENGINE2D_ALLOC(engine2d_alloc);

// TODO(pgm) This shouldn't live here?
#define ENGINE2D_FREE(name) void name(void *memory)
typedef ENGINE2D_FREE(engine2d_free);

namespace Engine2D
{
    struct EngineState 
    {
        char appTitle[MAX_APP_TITLE_LENGTH];
        // TODO(pgm)
        // Size screenSize;     
        ScreenRect visibleRegion;
        Tilemap tilemaps[MAX_TILEMAP_COUNT];
        u32 tilemapCount;
        Layer layers[MAX_LAYER_COUNT];
        u32 layerCount;

        // callbacks to the engine
        engine2d_alloc *malloc;
        engine2d_free  *free;
    };
}

#endif // ENGINE2D_STATE_H