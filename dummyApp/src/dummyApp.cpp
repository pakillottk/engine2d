#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <engine2d/application2d.h>
using namespace Engine2D;

internal void makeTexts(Layer *layer)
{
    layer->backgroundColor = { 0x0, 0x0, 0x0, 0x0 };
    layer->attributes.patternBackground = 0;
    layer->attributes.textLayer = 1;
    strcpy(layer->font.font, "arial.ttf");
    layer->font.fontSize = 12; 
    layer->textsCount = 2;
    layer->texts[0].screenNormalizedPosition = {0.5f, 0.5f};
    layer->texts[0].color = { 0xff, 0xff, 0xff, 0xff };
    layer->texts[0].text = (char*)calloc(6, sizeof(char));
    strcpy(layer->texts[0].text, "Hello");

    layer->texts[1].screenNormalizedPosition = {0.3f, 0.15f};
    layer->texts[1].color = { 0xff, 0xff, 0x00, 0xff };
    layer->texts[1].text = (char*)calloc(9, sizeof(char));
    strcpy(layer->texts[1].text, "Ayy lmao");
}

internal void makeCollidingSprites( EngineState *state )
{
    state->layers[0].spriteCount = 2;
    Sprite *sp1 = &state->layers[0].sprites[0];
    Sprite *sp2 = &state->layers[0].sprites[1];

    sp1->size = { 10, 10 };
    sp1->worldPosition = { 100, 100 };
    sp1->pixels = (ColorRGBA32*)malloc( sizeof(ColorRGBA32)*100 );
    memset(sp1->pixels, 0xff0000ff, sizeof(ColorRGBA32) * 100);

    sp2->size = { 10, 10 };
    sp2->worldPosition = { 107, 100 };
    sp2->pixels = (ColorRGBA32*)malloc( sizeof(ColorRGBA32)*100 );
    memset(sp1->pixels, 0x00ff00ff, sizeof(ColorRGBA32) * 100);
}

ENGINE2D_INITIALIZE_APPLICATION(Engine2D_InitializeApplication)
{
    strcpy(state->appTitle, "DummyApp"); 
    state->visibleRegion = { 0, 0, 800, 600 };

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

    state->layers[0].backgroundColor = { 0x66, 0x66, 0x66, 0xff };
    state->layerCount = 2;
    makeCollidingSprites(state);
    makeTexts(&state->layers[1]);
}

ENGINE2D_APPLICATION_UPDATE(Engine2D_ApplicationUpdate)
{
    keep_alive_var u32 speed = 1; 

    Sprite *sprite = &state->layers[0].sprites[0];
    sprite->worldPosition.x -= input->arrows.left * speed;
    sprite->worldPosition.x += input->arrows.right * speed;
    sprite->worldPosition.y += input->arrows.up * speed;
    sprite->worldPosition.y -= input->arrows.down * speed; 

    // state->visibleRegion.x = sprite->worldPosition.x - state->visibleRegion.width * 0.5f;
    // state->visibleRegion.y = sprite->worldPosition.y - state->visibleRegion.height * 0.5f;
}