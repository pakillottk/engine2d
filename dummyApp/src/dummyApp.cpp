#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <engine2d/application2d.h>

#include <Engine2D/Engine2D_App.h>
using namespace Engine2D;

class DummyApp : public App2D
{
public:
    DummyApp(Engine2D::EngineState *state);    
    void update(real32 deltaTime, real32 totalTime, Engine2D::EngineState *state, Engine2D::UserInput *input);
};

#define USE_OOP
#define APP_CLASS DummyApp
#include <Engine2D/Engine2D.h>

internal void makeTexts(EngineState *state, Layer *layer)
{
    layer->backgroundColor = { 0x0, 0x0, 0x0, 0x0 };
    layer->attributes.patternBackground = 0;
    layer->attributes.textLayer = 1;
    strcpy(layer->font.font, "c:\\windows\\fonts\\times.ttf");
    layer->font.fontSize = 24; 
    layer->textsCount = 2;
    layer->texts[0].screenNormalizedPosition = {0.5f, 0.5f};
    layer->texts[0].color = { 0xff, 0xff, 0xff, 0xff };
    layer->texts[0].text = (char*)state->malloc(6 * sizeof(char));
    strcpy(layer->texts[0].text, "Hello");

    layer->texts[1].screenNormalizedPosition = {0.3f, 0.15f};
    layer->texts[1].color = { 0xff, 0xff, 0x00, 0xff };
    layer->texts[1].text = (char*)state->malloc(9 * sizeof(char));
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

// ENGINE2D_INITIALIZE_APPLICATION(Engine2D_InitializeApplication)
DummyApp::DummyApp(EngineState *state): App2D(state)
{
    strcpy(state->appTitle, "DummyApp"); 
    state->visibleRegion = { 0, 0, 640, 320 };

    state->layers[0].backgroundColor = { 0x66, 0x66, 0x66, 0xff };
    state->layers[0].spriteCount = 1;
    loadImage("data/bg.jpg", state, &state->layers[0].sprites[0].pixels, &state->layers[0].sprites[0].size);

    state->tilemapCount = 1;
    Tilemap *tilemap = &state->tilemaps[0];
    loadImage("data/tileset.png", state, &tilemap->pixels, &tilemap->mapSize);
    tilemap->tileSize = {32, 32};

    state->layerCount = 4;
    Layer *layer = &state->layers[1];
    layer->tilesetId = 0;
    layer->attributes.useTilemap = true;
    layer->tileCount = 1;
    layer->tiles = (TileReference*)state->malloc( layer->tileCount * sizeof(TileReference) );

    layer = &state->layers[2];
    layer->spriteCount = 1;    
    loadImage("data/char.png", state, &layer->sprites[0].pixels, &layer->sprites[0].size);

    makeTexts(state, &state->layers[3]);
}

// ENGINE2D_APPLICATION_UPDATE(Engine2D_ApplicationUpdate)
void DummyApp::update(real32 deltaTime, real32 totalTime, EngineState *state, UserInput *input)
{
    keep_alive_var u32 speed = 60; 

    // Sprite *sprite = &state->layers[1].sprites[0];
    // sprite->worldPosition.x -= input->arrows.left * speed;
    // sprite->worldPosition.x += input->arrows.right * speed;
    // sprite->worldPosition.y += input->arrows.up * speed;
    // sprite->worldPosition.y -= input->arrows.down * speed; 

    state->visibleRegion.x -= input->buttons.left * speed * deltaTime;
    state->visibleRegion.x += input->buttons.right * speed * deltaTime;
    state->visibleRegion.y += input->buttons.up * speed * deltaTime;
    state->visibleRegion.y -= input->buttons.down * speed * deltaTime; 

    state->visibleRegion.width -= input->buttons.zoomPlus;
    state->visibleRegion.height -= input->buttons.zoomPlus;
    state->visibleRegion.width += input->buttons.zoomMinus;
    state->visibleRegion.height += input->buttons.zoomMinus;

    // state->visibleRegion.x = sprite->worldPosition.x - state->visibleRegion.width * 0.5f;
    // state->visibleRegion.y = sprite->worldPosition.y - state->visibleRegion.height * 0.5f;
}

#if 0
ENGINE2D_APPLICATION_UPDATE(Engine2D_QuitApplication)
{
    // TODO
}
#endif