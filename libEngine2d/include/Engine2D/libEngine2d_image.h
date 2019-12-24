#pragma once
#ifndef LIB_ENGINE2D_IMAGE_H
#define LIB_ENGINE2D_IMAGE_H

#include <engine2d/engine2d_sprite.h>

namespace Engine2D
{
    bool8 loadImage(const char *imgPath, ColorRGBA32** pixels, Size* size);
};


#endif // LIB_ENGINE2D_IMAGE_H