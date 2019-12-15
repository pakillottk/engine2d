#pragma once
#ifndef ENGINE2D_LAYER_H
#define ENGINE2D_LAYER_H

#include <stdlib.h>
#include "engine2d_types.h"
#include "engine2d_sprite.h"
#include "engine2d_text.h"
#include "engine2d_collision.h"

#define MAX_SPRITE_COUNT 128
#define MAX_TEXTS_COUNT 128
namespace Engine2D
{
    enum LAYER_FLAGS
    {
        bHidden             = 0b001,
        bPatternBackground  = 0b010,
        bTextLayer          = 0b100
    };

    struct Layer
    {
        // background
        union
        {
            ColorRGBA32 backgroundColor;
            struct 
            {
                ColorRGBA32 *backgroundPattern;
                Size patternSize;
            };
        };
        // layer data
        union 
        {
            // sprite data
            struct
            {
                Sprite sprites[MAX_SPRITE_COUNT];
                u32 spriteCount;
            };
            // texts data
            struct 
            {
                FontConfig font;
                Text texts[MAX_TEXTS_COUNT];
                u32 textsCount;
            };
        };
        // layer flags
        union 
        {
            u32 flags;
            struct 
            {
                byte hidden : 1;  
                byte patternBackground : 1;  
                byte textLayer : 1;  
            };
        } attributes;

        Collisions collisions;
    };

    inline void releaseLayer(Layer *layer)
    {
        if( layer->attributes.patternBackground )
        {
            free(layer->backgroundPattern);
        }
        if( layer->attributes.textLayer )
        {
            for( u32 i = 0; i < layer->textsCount; ++i )
            {
                releaseText( &layer->texts[i] );
            }
        }
        else
        {
            for( u32 i = 0; i < layer->spriteCount; ++i )
            {
                free(layer->sprites[i].pixels);
            }
            free(layer->collisions.CollisionData);
        }
    }
}

#endif // ENGINE2D_LAYER_H