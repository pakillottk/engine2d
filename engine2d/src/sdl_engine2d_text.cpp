#include <SDL2/SDL_TTF.h>
#include <stdio.h>
#include "engine2d_render_text.h"
using namespace Engine2D;

void renderText(FontConfig *font, Text *text, Size *screenSize, ColorRGBA32* buffer)
{
    // TODO(pgm) For now in each frame makes a new font. We have to create a font cache
    TTF_Font *ttfFont = TTF_OpenFont(font->font, font->fontSize);
    if( !ttfFont )
    {
        fprintf(stderr, "cant load the font: %s", font->font);
        return;
    }

    SDL_Color color = { 0xff, 0xff, 0xff, 0xff };
    SDL_Surface *surf = TTF_RenderText_Solid(ttfFont, text->text, color );
    SDL_Surface *rgbaSurf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, NULL);
    // copy the pixels into our buffer
    SDL_Rect rect = rgbaSurf->clip_rect;
    Rect textRect
    { 
        text->screenNormalizedPosition.x, 
        text->screenNormalizedPosition.y, 
        rect.w, 
        rect.h 
    };
    // TODO(pgm): handle the text alignent
    ScreenRect projectedRect = mapNormalizedRect(textRect, *screenSize);
    // drawScreenRect( projectedRect, *screenSize, (ColorRGBA32*)rgbaSurf->pixels, buffer );
    drawScreenRectMasked( projectedRect, *screenSize, text->color, (ColorRGBA32*)rgbaSurf->pixels, buffer );

    TTF_CloseFont(ttfFont);
    SDL_FreeSurface(surf);
    SDL_FreeSurface(rgbaSurf);
}