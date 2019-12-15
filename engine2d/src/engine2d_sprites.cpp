#include "../include/engine2d/engine2d_sprite.h"

internal ScreenRect spriteScreenRect(Sprite *sprite)
{
    ScreenRect rect 
    { 
        sprite->worldPosition.x, 
        sprite->worldPosition.y, 
        sprite->size.width, 
        sprite->size.height 
    };
    return(rect);
}