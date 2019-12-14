#pragma once
#ifndef APPLICATION_2D_H
#define APPLICATION_2D_H

#include <stdint.h>

typedef uint8_t bool8;

typedef int32_t i32;
typedef int64_t i64;

typedef uint32_t u32;
typedef uint64_t u64;

typedef float real32;
typedef double real64;

#define MAX_APP_TITLE_LENGTH 1024

struct EngineState 
{
    // TODO(pgm)
    char appTitle[1024];
    u32 clearColor;
};

struct UserInput
{
    // TODO(pgm)
};

#ifdef __cplusplus
extern "C" {
#endif

void Engine2D_InitializeApplication(EngineState *state);
void Engine2D_InputUpdated(EngineState *state, UserInput *input);

#ifdef __cplusplus
}
#endif

#endif // APPLICATION_2D_H