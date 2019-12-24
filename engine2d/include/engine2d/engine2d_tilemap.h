#pragma once
#ifndef ENGINE2D_TILEMAP_H
#define ENGINE2D_TILEMAP_H

#include "engine2d_types.h"
#include <string.h>

#define MAX_TILEMAP_COUNT 128

namespace Engine2D
{
    struct Tilemap
    {
        ColorRGBA32 *pixels;
        Size mapSize;
        Size tileSize;
    };

    struct TileReference
    {
        u32 tileIndex;
        Vec2i worldPos;
    };
    
    inline void releaseTilemap(Tilemap *tilemap)
    {
        free(tilemap->pixels);
    }
};

#endif // ENGINE2D_TILEMAP_H