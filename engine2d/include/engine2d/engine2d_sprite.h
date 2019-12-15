#pragma once
#ifndef ENGINE2D_SPRITE_H
#define ENGINE2D_SPRITE_H

#include "engine2d_types.h"

namespace Engine2D
{
    struct Sprite
    {
        ColorRGBA32 *pixels;
        Size size; 
        Vec2i worldPosition;
    };
}

#endif // ENGINE2D_SPRITE_H