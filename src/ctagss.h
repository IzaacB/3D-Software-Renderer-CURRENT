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
#define RENDER_DISTANCE 25

array_define_type(f32, f32_array);
array_define_type(u32, u32_array);
array_define_type(v3, v3_array);
array_define_type(v2, v2_array);

typedef struct
{
    f32 r, g, b;
}
color;

typedef struct
{
    u32 i0, i1, i2;
    u32 uv0, uv1, uv2;
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
    v3 direction;
    color c;
    f32 intensity;
}
dir_light;
array_define_type(dir_light, dir_light_array);

typedef struct
{
    SDL_Surface *source;
    u32 *pixels;
    u32 width, height;
    u32 h_frames, v_frames;
}
image;

typedef struct
{
    color c;
    bool textured;
    image texture;
}
material;
array_define_type(material, material_array);

typedef struct
{
    v3_array vertices;
    face_array faces;

    v3_array normals;
    v2_array uvs;

    material m;
    
    transform t;
}
object;

struct State
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *canvas;

    u32 surface[CANVAS_WIDTH * CANVAS_HEIGHT];
    f32 depth_buffer[CANVAS_WIDTH * CANVAS_HEIGHT];

    f32 delta;
    const u8 *keystate;
};

struct Scene
{
    v3_array vertices;
    face_array faces;
    v3_array normals;
    v2_array uvs;

    material_array materials;
    u32_array material_indices;

    v3_array projected;

    dir_light_array dir_lights;
};

struct Images
{
    image test;
};

struct Objects
{
    object cube;
    object sphere;
    object cone;
    object cylinder;
    object torus;
    object sponza_wall;
    object sponza_floor;
};

struct Viewport
{
    transform t;
    plane near, far, left, right, top, bottom;
};

extern struct State state;
extern struct Scene scene;
extern struct Images images;
extern struct Objects objects;
extern struct Viewport viewport;

#endif