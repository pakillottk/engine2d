#include "engine2d_rect.h"
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

internal ColorRGBA32 blendByAlpha(ColorRGBA32 destination, ColorRGBA32 source)
{
    ColorRGBA32 result;
    real32 alpha = real32(source.a)/255.0f;
    real32 oneMinusAlpha = 1.0f - alpha;
    result.r = (source.r * alpha) + (oneMinusAlpha * destination.r);
    result.g = (source.g * alpha) + (oneMinusAlpha * destination.g);
    result.b = (source.b * alpha) + (oneMinusAlpha * destination.b);
    result.a = 0xff;

    return(result);
}

bool8 rectOverlaps(Rect *rect1, Rect *rect2)
{
    return !(
            rect2->x > (rect1->x + rect1->width) || (rect2->x + rect2->width) < rect1->x 
            || (rect2->y + rect2->height) < rect1->y || rect2->y > (rect1->y + rect1->height)
        );
}

bool8 rectOverlaps(ScreenRect *rect1, ScreenRect *rect2)
{
    return !(
            rect2->x > (rect1->x + rect1->width) || (rect2->x + rect2->width) < rect1->x 
            || (rect2->y + rect2->height) < rect1->y || rect2->y > (rect1->y + rect1->height)
        );
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

            buffer[ (y * screenSize.width) + x ] = blendByAlpha(buffer[ (y * screenSize.width) + x ], color);
            // TODO(pgm): Fake blending
            // if( color.a != 0x0 )
            // {
            //     buffer[ (y * screenSize.width) + x ] = color;
            // }
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
            buffer[ (y * screenSize.width) + x ] = blendByAlpha(buffer[ (y * screenSize.width) + x ], colors[row * rect.width + col]);
            // TODO(pgm): Fake blending
            // if( colors[row * rect.width + col].a != 0 )
            // {
            //     buffer[ (y * screenSize.width) + x ] = colors[row * rect.width + col];
            // }
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

#define LERP(V1, V2, T) ((V1) + ((V2)-(V1)) * (T))
#define BLERP(V1, V2, V3, V4, T, S) LERP(LERP(V1, V2, T), LERP(V3, V4, T), S)  

internal ColorRGBA32 calculateColorBilinearLerp(ColorRGBA32 *colors, const Size &size, real32 rx, real32 ry)
{
    u32 x = (u32)rx;
    u32 y = (u32)ry;

    ColorRGBA32 color = colors[ y * size.width + x ];
    if( x < size.width - 1 && y < size.height - 1  )
    {
        // not a corner
        ColorRGBA32 p00 = color;
        ColorRGBA32 p10 = colors[ y * size.width + (x+1) ];
        ColorRGBA32 p01 = colors[ (y+1) * size.width + x ];
        ColorRGBA32 p11 = colors[ (y+1) * size.width + (x+1) ];
        for( char i = 0; i < 4; ++i )
        {
            color.color_rgba[i] = BLERP(p00.color_rgba[i], p10.color_rgba[i], p01.color_rgba[i], p11.color_rgba[i], rx - x, ry - y);
        }
    }
    return(color);
}

void drawScreenRect(const ScreenRect &rect, const Size &realSize, const Size &screenSize, ColorRGBA32* colors, ColorRGBA32 *buffer)
{
    // TODO(pgm) for now use nearest pixel
    int x, y;
    real32 sampleX, sampleY;
    real32 normalizedX, normalizedY;
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
            
            normalizedX = real32(col) / real32(rect.width);
            sampleX =  normalizedX * realSize.width;

            normalizedY = real32(row) / real32(rect.height);
            sampleY =  normalizedY * realSize.height;

            // const ColorRGBA32 &color = colors[sampleY * realSize.width + sampleX];
            const ColorRGBA32 &color = calculateColorBilinearLerp(colors, realSize, sampleX, sampleY);

            buffer[ (y * screenSize.width) + x ] = blendByAlpha(buffer[ (y * screenSize.width) + x ], color);
        }
    }
}

void drawScreenRect(const ScreenRect &rect, const Size &realSize, const Size &screenSize, u32 *colors, u32 *buffer)
{
    // TODO(pgm) for now use nearest pixel
    int x, y;
    u32 sampleX, sampleY;
    real32 normalizedX, normalizedY;
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
            
            normalizedX = real32(col) / real32(rect.width);
            sampleX =  normalizedX * realSize.width;

            normalizedY = real32(row) / real32(rect.height);
            sampleY =  normalizedY * realSize.height;

            const u32 &color = colors[sampleY * realSize.width + sampleX];

            buffer[ (y * screenSize.width) + x ] = blendByAlpha(buffer[ (y * screenSize.width) + x ], color);
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
    result.width = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.width) * screenSize.width;
    result.height = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.height) * screenSize.height;
    // TODO(pgm) no resizes for now
    // result.width = rect.width;
    // result.height = rect.height;
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