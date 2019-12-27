#include <stdio.h>
#include <string.h>

#include "engine2d_render_text.h"
#include "engine2d_rect.h"
#include "engine2d_io.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../../vendor/STB_TrueType/stb_truetype.h"

using namespace Engine2D;

internal Vec2i renderChar(char c, real32 fontSize, ColorRGBA32 *color, Vec2 *screenNormalizedPosition, int ascent, real32 hOffset, stbtt_fontinfo *stbFont, Size *screenSize, ColorRGBA32 *buffer)
{
    int minX, maxX, minY, maxY;
    int w, h, xOff, yOff;
    stbtt_GetCodepointBitmapBox(stbFont, (int)c, 0, fontSize, &minX, &minY, &maxX, &maxY);
    unsigned char *bitmap = stbtt_GetCodepointBitmap(stbFont, 0, fontSize, (int)c, &w, &h, &xOff, &yOff);
    ColorRGBA32 *pixels = (ColorRGBA32*)malloc( w * h * sizeof(ColorRGBA32) );

    ColorRGBA32 *pixel = pixels;
    unsigned char *bmp = bitmap;    
    for( u32 i = 0; i < w * h; ++i )
    {
        *pixel = *color;
        pixel->a = *bmp;        
        ++pixel;
        ++bmp;
    }
    
    Rect txtRect 
    {
        screenNormalizedPosition->x,
        screenNormalizedPosition->y,
        w,
        h
    };
    ScreenRect txtViewportRect = mapNormalizedRect(txtRect, *screenSize);
    txtViewportRect.x += hOffset;
    txtViewportRect.y += ascent + maxY;
    drawScreenRect(txtViewportRect, *screenSize, pixels, buffer);

    free(pixels);
    stbtt_FreeBitmap(bitmap, NULL);

    return Vec2i { w, h };
}

void renderText(FontConfig *font, Text *text, Size *screenSize, ColorRGBA32* buffer)
{
    char *fontFileData = readEntireFile(font->font);

    stbtt_fontinfo stbFont;
    if( !stbtt_InitFont(&stbFont, (unsigned char*)fontFileData, 0) )
    {
        // TODO(pgm) Font loading failed...        
        return;
    }

    float scale = stbtt_ScaleForPixelHeight(&stbFont, font->fontSize);
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&stbFont, &ascent, &descent, &lineGap);
    ascent  *= scale;
    descent *= scale;

    Vec2i accumOffset = {};
    size_t len = strlen(text->text);
    for( size_t i = 0; i < len; ++i )    
    {
        if( text->text[i] == ' ' )
        {
            accumOffset.x += 6;
        }
        else
        {
            Vec2i size = renderChar(
                text->text[i], 
                scale, 
                &text->color, 
                &text->screenNormalizedPosition, 
                ascent, 
                accumOffset.x, 
                &stbFont, 
                screenSize, 
                buffer
            );
            accumOffset.x += size.x + 2;
        }
    }

    free(fontFileData);
}