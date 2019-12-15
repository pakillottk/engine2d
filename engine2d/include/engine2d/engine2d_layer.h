#pragma once
#ifndef ENGINE2D_LAYER_H
#define ENGINE2D_LAYER_H

#include "engine2d_types.h"
#include "engine2d_sprite.h"

#define MAX_SPRITE_COUNT 128
namespace Engine2D
{
    enum LAYER_FLAGS
    {
        bHidden             = 0b001,
        bPatternBackground  = 0b010
    };

    struct Layer
    {
        ColorRGBA32 backgroundColor;
        ColorRGBA32 *backgroundPattern;
        Size patternSize;
        union 
        {
            u32 flags;
            struct 
            {
                byte hidden : 1;  
                byte patternBackground : 1;  
            };
        } attributes;
        Sprite sprites[MAX_SPRITE_COUNT];
        u32 spriteCount;
    };

    inline void releaseLayer(Layer *layer)
    {
        if( layer->attributes.patternBackground )
        {
            free(layer->backgroundPattern);
        }
        for( u32 i = 0; i < layer->spriteCount; ++i )
        {
            free(layer->sprites[i].pixels);
        }
    }
}

#endif // ENGINE2D_LAYER_H