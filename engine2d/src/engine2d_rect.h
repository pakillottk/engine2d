#pragma once
#ifndef ENGINE2D_RECT_H
#define ENGINE2D_RECT_H

#include "../include/engine2d/engine2d_types.h"

bool8 rectOverlaps(Engine2D::Rect *rect1, Engine2D::Rect *rect2);
bool8 rectOverlaps(Engine2D::ScreenRect *rect1, Engine2D::ScreenRect *rect2);

void drawScreenRect(const Engine2D::ScreenRect &rect, const Engine2D::Size &screenSize, Engine2D::ColorRGBA32 color, Engine2D::ColorRGBA32 *buffer);
void drawScreenRect(const Engine2D::ScreenRect &rect, const Engine2D::Size &screenSize, u32 color, u32 *buffer);
void drawScreenRect(const Engine2D::ScreenRect &rect, const Engine2D::Size &screenSize, Engine2D::ColorRGBA32* colors, Engine2D::ColorRGBA32 *buffer);
void drawScreenRect(const Engine2D::ScreenRect &rect, const Engine2D::Size &screenSize, u32* colors, u32 *buffer);
void drawScreenRect(const Engine2D::ScreenRect &rect, const Engine2D::Size &realSize, const Engine2D::Size &screenSize, Engine2D::ColorRGBA32* colors, Engine2D::ColorRGBA32 *buffer);
void drawScreenRect(const Engine2D::ScreenRect &rect, const Engine2D::Size &realSize, const Engine2D::Size &screenSize, u32 *colors, u32 *buffer);

void drawScreenRectMasked(const Engine2D::ScreenRect &rect, const Engine2D::Size &screenSize, const Engine2D::ColorRGBA32 &color, Engine2D::ColorRGBA32* mask, Engine2D::ColorRGBA32 *buffer);

void drawScreenRectPattern(const Engine2D::ScreenRect &rect, const Engine2D::Size &screenSize, Engine2D::ColorRGBA32* colors, const Engine2D::Size &patternSize, Engine2D::ColorRGBA32 *buffer); 
void drawScreenRectPattern(const Engine2D::ScreenRect &rect, const Engine2D::Size &screenSize, u32* colors, const Engine2D::Size &patternSize, u32 *buffer);

void scaleRect(Engine2D::Rect &rect, real32 scale);

Engine2D::ScreenRect mapRectToScreen(const Engine2D::Rect &rect, const Engine2D::Size &screenSize, const Engine2D::Rect &gameRect);
Engine2D::ScreenRect mapScreenRectToViewport(const Engine2D::ScreenRect &rect, const Engine2D::Size &screenSize, const Engine2D::ScreenRect &gameRect);
Engine2D::ScreenRect mapNormalizedRect(Engine2D::Rect &rect, const Engine2D::Size &screenSize);

Engine2D::ScreenRect spriteScreenRect(Engine2D::Sprite *sprite);

#endif // ENGINE2D_RECT_H