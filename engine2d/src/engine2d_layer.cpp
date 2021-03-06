#include "../include/engine2d/engine2d_layer.h"
#include "engine2d_rect.h"
using namespace Engine2D;

internal bool8 getTile(Tilemap *tilemap, u32 id, ColorRGBA32 *buffer)
{
    // TODO(pgm) validate the id    

    u32 tilesPerRow = tilemap->mapSize.width / tilemap->tileSize.width;
    u32 tileRow = id / tilesPerRow;
    u32 tileCol = id % tilesPerRow;
    u32 tileStart = tileRow * tilemap->tileSize.height * tilemap->mapSize.width + tileCol * tilemap->tileSize.width;

    ColorRGBA32 *tile = &tilemap->pixels[tileStart];
    ColorRGBA32 *pixel = buffer;
    for( u32 row = 0; row < tilemap->tileSize.height; ++row )
    {        
        for( u32 col = 0; col < tilemap->tileSize.width; ++col )
        {
            *pixel = tile[row * tilemap->mapSize.width + col];
            ++pixel;
        }
    }

    return(true);
}

internal void renderLayer(Layer *layer, ColorRGBA32 *buffer, Size *screenSize, ScreenRect *visibleRegion, Tilemap *tilemaps, u32 tilemapCount)
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
    
    if( layer->attributes.useTilemap )
    {
        // layer is tiled. Render with tilemaps
        // TODO(pgm) For now is done this way... The good way will have a function capable of giving us a screenrect from a tile
        // and worldpos
        Tilemap *layerTilemap = &tilemaps[layer->tilesetId];
        ColorRGBA32 *tilemapBuffer = (ColorRGBA32*)malloc( layerTilemap->tileSize.width * layerTilemap->tileSize.height * sizeof(ColorRGBA32) );
        for( u32 i = 0; i < layer->tileCount; ++i )
        {
            TileReference *tileRef = &layer->tiles[i];
            ScreenRect tileRect = { tileRef->worldPos.x, tileRef->worldPos.y, layerTilemap->tileSize.width, layerTilemap->tileSize.height };
            ScreenRect projectedRect = mapScreenRectToViewport( tileRect, *screenSize, *visibleRegion );
            // only if its visible
            if( rectOverlaps(&projectedRect, &viewport) ) 
            {
                getTile(layerTilemap, tileRef->tileIndex, tilemapBuffer);
                drawScreenRect( projectedRect, layerTilemap->tileSize, *screenSize, tilemapBuffer, buffer );
            }
        }
        free(tilemapBuffer);
    }
    else if( layer->attributes.textLayer )
    {        
        // render contains text
        for( u32 i = 0; i < layer->textsCount; ++i )
        {
            renderText( &layer->font, &layer->texts[i], screenSize, buffer );
        }
    }
    else
    {
        // layer contains sprites
        for( u32 i = 0; i < layer->spriteCount; ++i )
        {
            Sprite *sprite = &layer->sprites[i];
            ScreenRect rect = spriteScreenRect( sprite );
            ScreenRect projectedRect = mapScreenRectToViewport( rect, *screenSize, *visibleRegion );

            // only if its visible
            if( rectOverlaps(&projectedRect, &viewport) ) 
            {
                drawScreenRect( projectedRect, sprite->size, *screenSize, sprite->pixels, buffer );
            }
        }
    }
    
}