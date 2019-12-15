#include "../include/Engine2D/libEngine2d_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/STB_Image/stb_image.h"
using namespace Engine2D;

bool8 Engine2D::loadSpriteFromImg(const char *imgPath, Sprite *sprite)
{
    int w, h, channelCount;
    stbi_uc *imgData = stbi_load(imgPath, &w, &h, &channelCount, 0);
    if( imgData == NULL || channelCount > 4)
    {
        return(false);
    }

    sprite->size = { u32(w), u32(h) };
    sprite->pixels = (ColorRGBA32*)malloc( w * h * sizeof(ColorRGBA32) );
    stbi_uc *pixel = imgData;
    for( u32 i = 0; i < w * h; ++i )
    {
        for( u32 c = 0; c < channelCount; ++c )
        {
            sprite->pixels[i].color_rgba[c] = (u8)(*pixel);
            ++pixel;
        }
    }

    stbi_image_free(imgData);
    return(true);
}