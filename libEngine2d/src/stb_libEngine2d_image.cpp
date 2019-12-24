#include "../include/Engine2D/libEngine2d_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/STB_Image/stb_image.h"
using namespace Engine2D;

bool8 Engine2D::loadImage(const char *imgPath, ColorRGBA32** pixels, Size* size)
{
    int w, h, channelCount;
    stbi_uc *imgData = stbi_load(imgPath, &w, &h, &channelCount, 0);
    if( imgData == NULL || channelCount > 4)
    {
        return(false);
    }

    size->width = w;
    size->height = h;
    *pixels = (ColorRGBA32*)calloc( w * h, sizeof(ColorRGBA32) );
    stbi_uc *pixel = imgData;
    for( u32 i = 0; i < w * h; ++i )
    {
        for( u32 c = 0; c < channelCount; ++c )
        {
            (*pixels)[i].color_rgba[c] = (u8)(*pixel);
            ++pixel;
        }
    }

    stbi_image_free(imgData);
    return(true);
}