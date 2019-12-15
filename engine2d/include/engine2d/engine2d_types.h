#pragma once
#ifndef ENGINE2D_TYPES_H
#define ENGINE2D_TYPES_H

#define internal static
#define keep_alive_var static

#include <stdint.h>

typedef uint8_t bool8;
typedef uint8_t byte;
typedef byte color_rgba32[4];

typedef int8_t i8;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float real32;
typedef double real64;

namespace Engine2D
{
    union ColorRGBA32
    {
        color_rgba32 color_rgba;
        struct
        {
            byte r;
            byte g;
            byte b;
            byte a;
        };
    };

    struct Size
    {
        u32 width;
        u32 height;
    };

    struct Vec2i
    {
        i32 x;
        i32 y;
    };

    struct Vec2
    {
        real32 x;
        real32 y;
    };

    struct ScreenRect
    {
        i32 x;
        i32 y;
        u32 width;
        u32 height;
    };

    struct Rect
    {
        real32 x;
        real32 y;
        real32 width;
        real32 height;
    };
}


#endif // ENGINE2D_TYPES_H