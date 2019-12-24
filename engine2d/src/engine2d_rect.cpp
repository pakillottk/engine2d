#include "engine2d_project_rect.h"
using namespace Engine2D;

#define NORMALIZE_TO_RANGE(value, min, max) ( (value) - (min) ) / ( (max) - (min) )

internal u32 blendByAlpha(u32 c1, u32 c2)
{
    u8 a1 = 0xff & c1;
    u8 a2 = 0xff & c2;

    u32 res = c1 * a1 + c2 * a2;
    res |= 0x000000ff;
    return(res);
}

void drawScreenRect(const ScreenRect &rect, const Size &screenSize, ColorRGBA32 color, ColorRGBA32 *buffer)
{
    int x, y;
    for(unsigned row = 0; row < rect.height; ++row)
    {
        for(unsigned col = 0; col < rect.width; ++col)
        {
            x = rect.x + col;
            y = row + rect.y;
            // TODO(pgm): This can be optimized to get first the subrect that is in bounds
            if( x < 0 || x >= screenSize.width || y < 0 || y >= screenSize.height )
            {
                // this pixel it's out of bounds
                continue;
            }
            // TODO(pgm): Fake blending
            if( color.a != 0x0 )
            {
                buffer[ (y * screenSize.width) + x ] = color;
            }
        }
    }
}

void drawScreenRect(const ScreenRect &rect, const Size &screenSize, u32 color, u32 *buffer)
{
    int x, y;
    for(unsigned row = 0; row < rect.height; ++row)
    {
        for(unsigned col = 0; col < rect.width; ++col)
        {
            x = rect.x + col;
            y = row + rect.y;
            // TODO(pgm): This can be optimized to get first the subrect that is in bounds
            if( x < 0 || x >= screenSize.width || y < 0 || y >= screenSize.height )
            {
                // this pixel it's out of bounds
                continue;
            }
            
            buffer[ (y * screenSize.width) + x ] = color;
        }
    }
}

void drawScreenRect(const ScreenRect &rect, const Size &screenSize, ColorRGBA32* colors, ColorRGBA32 *buffer)
{
    int x, y;
    for(unsigned row = 0; row < rect.height; ++row)
    {
        for(unsigned col = 0; col < rect.width; ++col)
        {
            x = rect.x + col;
            y = row + rect.y;
            // TODO(pgm): This can be optimized to get first the subrect that is in bounds
            if( x < 0 || x >= screenSize.width || y < 0 || y >= screenSize.height )
            {
                // this pixel it's out of bounds
                continue;
            }
            // TODO(pgm): Fake blending
            if( colors[row * rect.width + col].a != 0x0 )
            {
                buffer[ (y * screenSize.width) + x ] = colors[row * rect.width + col];
            }
        }
    }
}

void drawScreenRect(const ScreenRect &rect, const Size &screenSize, u32* colors, u32 *buffer)
{
    int x, y;
    for(unsigned row = 0; row < rect.height; ++row)
    {
        for(unsigned col = 0; col < rect.width; ++col)
        {
            x = rect.x + col;
            y = row + rect.y;
            // TODO(pgm): This can be optimized to get first the subrect that is in bounds
            if( x < 0 || x >= screenSize.width || y < 0 || y >= screenSize.height )
            {
                // this pixel it's out of bounds
                continue;
            }
            buffer[ (y * screenSize.width) + x ] = colors[row * rect.width + col];
        }
    }
}

void drawScreenRectMasked(const ScreenRect &rect, const Size &screenSize, const ColorRGBA32 &color, ColorRGBA32* mask, ColorRGBA32 *buffer)
{
    int x, y;
    ColorRGBA32 pixelColor, maskValue;
    for(unsigned row = 0; row < rect.height; ++row)
    {
        for(unsigned col = 0; col < rect.width; ++col)
        {
            x = rect.x + col;
            y = row + rect.y;
            // TODO(pgm): This can be optimized to get first the subrect that is in bounds
            if( x < 0 || x >= screenSize.width || y < 0 || y >= screenSize.height )
            {
                // this pixel it's out of bounds
                continue;
            }
            maskValue = mask[row * rect.width + col];
            pixelColor.r = color.r & maskValue.r;
            pixelColor.g = color.g & maskValue.g;
            pixelColor.b = color.b & maskValue.b;
            pixelColor.a = color.a & maskValue.a;
            // TODO(pgm): Fake blending
            if( pixelColor.a != 0x0 )
            {
                buffer[ (y * screenSize.width) + x ] = pixelColor;
            }
        }
    }
}

void drawScreenRectPattern(const ScreenRect &rect, const Size &screenSize, ColorRGBA32* colors, const Size &patternSize, ColorRGBA32 *buffer)
{
    int x, y;
    for(unsigned row = 0; row < rect.height; ++row)
    {
        for(unsigned col = 0; col < rect.width; ++col)
        {
            x = rect.x + col;
            y = row + rect.y;
            // TODO(pgm): This can be optimized to get first the subrect that is in bounds
            if( x < 0 || x >= screenSize.width || y < 0 || y >= screenSize.height )
            {
                // this pixel it's out of bounds
                continue;
            }
            buffer[ (y * screenSize.width) + x ] = colors[((row % patternSize.height) * patternSize.width) + (col % patternSize.width)];
        }
    }
} 

void drawScreenRectPattern(const ScreenRect &rect, const Size &screenSize, u32* colors, const Size &patternSize, u32 *buffer)
{
    int x, y;
    for(unsigned row = 0; row < rect.height; ++row)
    {
        for(unsigned col = 0; col < rect.width; ++col)
        {
            x = rect.x + col;
            y = row + rect.y;
            // TODO(pgm): This can be optimized to get first the subrect that is in bounds
            if( x < 0 || x >= screenSize.width || y < 0 || y >= screenSize.height )
            {
                // this pixel it's out of bounds
                continue;
            }
            buffer[ (y * screenSize.width) + x ] = colors[((row % patternSize.height) * patternSize.width) + (col % patternSize.width)];
        }
    }
} 

void scaleRect(Rect &rect, real32 scale)
{
    real32 centerX = rect.x + rect.width  * 0.5f;
    real32 centerY = rect.y + rect.height * 0.5f;

    real32 targetWidth  = rect.width * scale;
    real32 targetHeight = rect.height * scale;

    rect.x = centerX - targetWidth * 0.5f;
    rect.y = centerY - targetHeight * 0.5f;
    rect.width = targetWidth;
    rect.height = targetHeight;
}

ScreenRect mapRectToScreen(const Rect &rect, const Size &screenSize, const Rect &gameRect)
{
    ScreenRect result;
    // TODO(pgm) no resizes for now
    // result.width = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.width) * screenSize.width;
    // result.height = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.height) * screenSize.height;
    result.width = rect.width;
    result.height = rect.height;
    result.x = NORMALIZE_TO_RANGE(rect.x, gameRect.x, gameRect.x + gameRect.width) * screenSize.width;
    result.y = screenSize.height - result.height - NORMALIZE_TO_RANGE(rect.y, gameRect.y, gameRect.y + gameRect.height) * screenSize.height;

    return(result);
}

ScreenRect mapScreenRectToViewport(const ScreenRect &rect, const Size &screenSize, const ScreenRect &gameRect)
{
    Rect rectReal { rect.x, rect.y, rect.width, rect.height };
    Rect gameRectReal { gameRect.x, gameRect.y, gameRect.width, gameRect.height };
    return mapRectToScreen(rectReal, screenSize, gameRectReal);
}

ScreenRect mapNormalizedRect(Rect &rect, const Size &screenSize)
{
    ScreenRect result;
    result.width = rect.width;
    result.height = rect.height;
    result.x = rect.x * screenSize.width;
    result.y = screenSize.height - rect.height - rect.y * screenSize.height;

    return(result);
}

ScreenRect spriteScreenRect(Sprite *sprite)
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