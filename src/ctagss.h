//Convienient Type and Global Struct Storage.

#ifndef CTAGSS_H
#define CTAGSS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>

#include "array.h"
#include "vector.h"

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
#define TARGET_FPS 60

array_define_type(f32, f32_array);
array_define_type(v3, v3_array);

typedef struct
{
    f32 r, g, b;
}
color;

typedef struct
{
    u32 i0, i1, i2;
}
face;
array_define_type(face, face_array);

typedef struct
{
    v3 normal;
    f32 distance;
}
plane;

typedef struct
{
    v3_array vertices;
    face_array faces;

    v3_array normals;
    
    transform t;
}
object;

struct State
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *canvas;

    u32 surface[CANVAS_WIDTH * CANVAS_HEIGHT];

    f32 delta;
    const u8 *keystate;
};

struct Scene
{
    v3_array vertices;
    face_array faces;
    v3_array normals;

    v3_array projected;
};

struct Objects
{
    object cube;
};

struct Viewport
{
    transform t;
    plane near, far, left, right, top, bottom;
};

extern struct State state;
extern struct Scene scene;
extern struct Objects objects;
extern struct Viewport viewport;

#endif