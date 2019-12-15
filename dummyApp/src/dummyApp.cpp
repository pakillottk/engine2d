#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <engine2d/application2d.h>
using namespace Engine2D;

ENGINE2D_INITIALIZE_APPLICATION(Engine2D_InitializeApplication)
{
    strcpy(state->appTitle, "DummyApp"); 
    state->visibleRegion = { 0, 0, 800, 600 };

    state->layerCount = 1;
    state->layers[0].backgroundColor = { 0x66, 0x66, 0x66, 0xff };
    // state->layers[0].backgroundPattern = (ColorRGBA32*)calloc(100, sizeof(ColorRGBA32));
    // for(u32 i = 0; i < 100; ++i)
    // {
    //     state->layers[0].backgroundPattern[i].r = i+50;
    //     state->layers[0].backgroundPattern[i].g = i+50;
    //     state->layers[0].backgroundPattern[i].b = i+50;
    //     state->layers[0].backgroundPattern[i].a = 0xff;
    // }
    // state->layers[0].patternSize = { 10, 10};
    // state->layers[0].attributes.patternBackground = 0;

    // state->layers[0].spriteCount = 1;
    // state->layers[0].sprites[0].pixels = (ColorRGBA32*)calloc(100, sizeof(ColorRGBA32));
    // for(u32 i = 0; i < 100; ++i)
    // {
    //     state->layers[0].sprites[0].pixels[i].r = 0xff;
    // }
    // state->layers[0].sprites[0].size = { 10, 10 };
    // state->layers[0].sprites[0].worldPosition = { 0, 0 };

    state->layers[0].attributes.patternBackground = 0;
    state->layers[0].attributes.textLayer = 1;
    strcpy(state->layers[0].font.font, "arial.ttf");
    state->layers[0].font.fontSize = 12; 
    state->layers[0].textsCount = 2;
    state->layers[0].texts[0].screenNormalizedPosition = {0.5f, 0.5f};
    state->layers[0].texts[0].color = { 0x0, 0x0, 0x0, 0xff };
    state->layers[0].texts[0].text = (char*)calloc(6, sizeof(char));
    strcpy(state->layers[0].texts[0].text, "Hello");

    state->layers[0].texts[1].screenNormalizedPosition = {0.3f, 0.15f};
    state->layers[0].texts[1].color = { 0xff, 0xff, 0x00, 0xff };
    state->layers[0].texts[1].text = (char*)calloc(9, sizeof(char));
    strcpy(state->layers[0].texts[1].text, "Ayy lmao");
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