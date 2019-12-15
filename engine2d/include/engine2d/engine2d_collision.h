#pragma once
#ifndef ENGINE2D_COLLISION_H
#define ENGINE2D_COLLISION_H

#include "engine2d_sprite.h"

namespace Engine2D
{
    struct CollisionData
    {
        Sprite *colliding[2];
        Vec2    worldPosition;
        Vec2i   screenPosition;
    };

    struct Collisions
    {
        CollisionData *CollisionData;
        u32 collisionCount;
    };
    
};



#endif // ENGINE2D_COLLISION_H