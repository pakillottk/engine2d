#pragma once
#ifndef ENGINE2D_RENDER_TEXT_H
#define ENGINE2D_RENDER_TEXT_H

#include "../include/engine2d/engine2d_text.h"

void renderText(Engine2D::FontConfig *font, Engine2D::Text *text, Size *screenSize, Engine2D::ColorRGBA32* buffer);

#endif //ENGINE2D_RENDER_TEXT_H