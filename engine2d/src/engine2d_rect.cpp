#include "../include/engine2d/engine2d_types.h"
using namespace Engine2D;
#define internal static

#define NORMALIZE_TO_RANGE(value, min, max) ( (value) - (min) ) / ( (max) - (min) )

internal void drawScreenRect(const ScreenRect &rect, const Size &screenSize, u32 color, u32 *buffer)
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

internal void drawScreenRect(const ScreenRect &rect, const Size &screenSize, u32* colors, u32 *buffer)
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

internal void drawScreenRectPattern(const ScreenRect &rect, const Size &screenSize, u32* colors, const Size &patternSize, u32 *buffer)
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

internal inline real32 screenRectAspectRatio(const ScreenRect &rect)
{
    return real32(rect.width) / real32(rect.height);
}

internal inline real32 rectAspectRatio(const Rect &rect)
{
    return rect.width / rect.height;
}

internal void scaleRect(Rect &rect, real32 scale)
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

internal ScreenRect mapRectToScreen(const Rect &rect, const Size &screenSize, const Rect &gameRect)
{
    ScreenRect result;
    result.width = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.width) * screenSize.width;
    result.height = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.height) * screenSize.height;
    result.x = NORMALIZE_TO_RANGE(rect.x, gameRect.x, gameRect.x + gameRect.width) * screenSize.width;
    result.y = screenSize.height - result.height - NORMALIZE_TO_RANGE(rect.y, gameRect.y, gameRect.y + gameRect.height) * screenSize.height;

    return(result);
}

internal ScreenRect mapScreenRectToViewport(const ScreenRect &rect, const Size &screenSize, const ScreenRect &gameRect)
{
    // ScreenRect result;
    // result.width = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.width) * screenSize.width;
    // result.height = NORMALIZE_TO_RANGE(rect.width, 0, gameRect.height) * screenSize.height;
    // result.x = NORMALIZE_TO_RANGE(rect.x, gameRect.x, gameRect.x + gameRect.width) * screenSize.width;
    // result.y = screenSize.height - result.height - NORMALIZE_TO_RANGE(rect.y, gameRect.y, gameRect.y + gameRect.height) * screenSize.height;
    Rect rectReal { rect.x, rect.y, rect.width, rect.height };
    Rect gameRectReal { gameRect.x, gameRect.y, gameRect.width, gameRect.height };
    return mapRectToScreen(rectReal, screenSize, gameRectReal);
}