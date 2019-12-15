#include "../include/engine2d/engine2d_layer.h"
#include "engine2d_project_rect.h"
using namespace Engine2D;

internal void renderLayer(Layer *layer, ColorRGBA32 *buffer, Size *screenSize, ScreenRect *visibleRegion)
{
    // TODO(pgm) Blend the layers
    ScreenRect viewport { 0, 0, screenSize->width, screenSize->height };
    if( layer->attributes.patternBackground )
    {
        drawScreenRectPattern( 
            viewport, 
            *screenSize, 
            layer->backgroundPattern,  
            layer->patternSize,
            buffer
        );
    }
    else
    {
        drawScreenRect( viewport, *screenSize, layer->backgroundColor, buffer );
    }
    
    if( layer->attributes.textLayer )
    {
        for(u32 i = 0; i < layer->textsCount; ++i)
        {
            renderText( &layer->font, &layer->texts[i], screenSize, buffer );
        }
    }
    else
    {
        for(u32 i = 0; i < layer->spriteCount; ++i)
        {
            Sprite *sprite = &layer->sprites[i];
            ScreenRect rect = spriteScreenRect( sprite );
            ScreenRect projectedRect = mapScreenRectToViewport( rect, *screenSize, *visibleRegion );
            drawScreenRect( projectedRect, *screenSize, sprite->pixels, buffer );
        }
    }
    
}