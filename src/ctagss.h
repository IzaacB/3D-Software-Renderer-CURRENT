#ifndef CTAGSS_H
#define CTAGSS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define CANVAS_WIDTH 320
#define CANVAS_HEIGHT 200

typedef struct
{
    f32 r, g, b;
}
color;

struct State
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *canvas;

    u32 surface[CANVAS_WIDTH * CANVAS_HEIGHT];
};

extern struct State state;

#endif