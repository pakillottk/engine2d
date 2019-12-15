#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <engine2d/application2d.h>
using namespace Engine2D;

#define internal static

ENGINE2D_INITIALIZE_APPLICATION(Engine2D_InitializeApplication)
{
    strcpy(state->appTitle, "DummyApp"); 
    state->visibleRegion = { 0, 0, 800, 600 };

    state->layerCount = 1;
    state->layers[0].backgroundColor = { 0x0, 0x0, 0x0, 0x0 };
    state->layers[0].backgroundPattern = (ColorRGBA32*)calloc(100, sizeof(ColorRGBA32));
    for(u32 i = 0; i < 100; ++i)
    {
        state->layers[0].backgroundPattern[i].r = i+50;
        state->layers[0].backgroundPattern[i].g = i+50;
        state->layers[0].backgroundPattern[i].b = i+50;
        state->layers[0].backgroundPattern[i].a = 0xff;
    }
    state->layers[0].patternSize = { 10, 10};
    state->layers[0].attributes.patternBackground = 0;

    state->layers[0].spriteCount = 1;
    state->layers[0].sprites[0].pixels = (ColorRGBA32*)calloc(100, sizeof(ColorRGBA32));
    for(u32 i = 0; i < 100; ++i)
    {
        state->layers[0].sprites[0].pixels[i].r = 0xff;
    }
    state->layers[0].sprites[0].size = { 10, 10 };
    state->layers[0].sprites[0].worldPosition = { 0, 0 };
}

ENGINE2D_APPLICATION_UPDATE(Engine2D_ApplicationUpdate)
{
    u32 speed = 3; 

    Sprite *sprite = &state->layers[0].sprites[0];
    sprite->worldPosition.x -= input->arrows.left * speed;
    sprite->worldPosition.x += input->arrows.right * speed;
    sprite->worldPosition.y += input->arrows.up * speed;
    sprite->worldPosition.y -= input->arrows.down * speed; 

    // state->visibleRegion.x = sprite->worldPosition.x - state->visibleRegion.width * 0.5f;
    // state->visibleRegion.y = sprite->worldPosition.y - state->visibleRegion.height * 0.5f;
}