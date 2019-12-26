#include "../include/engine2d/engine2d_collision.h"
#include "../include/engine2d/engine2d_layer.h"
#include "engine2d_rect.h"
#include <vector>
using namespace Engine2D;

Collisions calculateCollisions(Layer *layer, Sprite **spriteMap, Size *screenSize, ScreenRect *visibleRegion)
{
    Collisions result;
    result.collisionCount = 0;

    std::vector<CollisionData> collisions;
    CollisionData collisionData;
    Sprite **screenSprite = NULL;
    int x, y;
    for(u32 i = 0; i < layer->spriteCount; ++i)
    {
        Sprite *sp = &layer->sprites[i];
        ScreenRect spRect = spriteScreenRect(sp);
        ScreenRect projectedRect = mapScreenRectToViewport( spRect, *screenSize, *visibleRegion );
        
        for( u32 row = 0; row < projectedRect.height; ++row )
        {
            for( u32 col = 0; col < projectedRect.width; ++col )
            {
                x = projectedRect.x + col;
                y = projectedRect.y + row;
                // TODO(pgm): This can be optimized to get first the subrect that is in bounds
                if( x < 0 || x >= screenSize->width || y < 0 || y >= screenSize->height )
                {
                    // this pixel it's out of bounds
                    continue;
                }
         
                screenSprite = &spriteMap[ y*screenSize->width + x ];

                if( *screenSprite == NULL )
                {
                    // no collision, is free
                    *screenSprite = sp;
                }
                else
                {
                    // is occupied. We have a collision
                    collisionData.colliding[0] = *screenSprite;
                    collisionData.colliding[1] = sp;
                    collisionData.screenPosition = { x, y };
                    collisionData.worldPosition = { spRect.x + col, spRect.y + row };
                    collisions.push_back(collisionData);

                    ++result.collisionCount;
                }
            }
        }
    }

    result.CollisionData = (CollisionData*)malloc(sizeof(CollisionData)*result.collisionCount);
    std::copy( collisions.begin(), collisions.end(), result.CollisionData );

    return(result);
}