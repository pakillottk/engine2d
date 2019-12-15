#pragma once
#ifndef ENGINE2D_TEXT_H
#define ENGINE2D_TEXT_H

#include "engine2d_types.h"
#include <stdlib.h>

namespace Engine2D
{
    struct FontConfig
    {
        char font[1024];
        u32 fontSize;

        // TODO
        // bool8 underline;
        // bool8 outline;
        // u32 outlineThickness;
    };

    struct Text
    {
        char *text;
        ColorRGBA32 color;

        enum
        {
            LEFT_ALIGNED    = -1,
            CENTER_ALIGNED  = 0,
            RIGHT_ALIGNED   = 1
        } TextAlign;
        
        // between [0.0, 1.0] so the text could be positionated fluidly on the screen
        Vec2 screenNormalizedPosition;
    };

    inline void releaseText(Text *text)
    {
        free(text->text);
    }
}

#endif // ENGINE2D_TEXT_H