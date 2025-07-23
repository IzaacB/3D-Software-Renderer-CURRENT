#include "ctagss.h"
#include "render.h"
#include "scene.h"
#include "raster.h"
#include "object.h"
#include "image.h"

void render_init()
{
    state.window = SDL_CreateWindow(
        "Software Rasterizer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 800,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    state.renderer = SDL_CreateRenderer(
        state.window,
        -1,
        SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC
    );

     state.canvas = SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        CANVAS_WIDTH, CANVAS_HEIGHT
    );
}

void render_update()
{
    SDL_UpdateTexture(state.canvas, NULL, state.surface, CANVAS_WIDTH * 4);
    SDL_RenderCopyEx(state.renderer, state.canvas, NULL, NULL, 0.0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(state.renderer);
}

void render_clear()
{
    memset(state.surface, 0, sizeof(state.surface));
}

void render_quit()
{
    SDL_DestroyTexture(state.canvas);
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

void render_preload()
{
    images.test = image_load("../res/models/sponza/textures/wall.bmp");

    objects.cube = object_import("../res/models/primitives/cube.obj", 0);
    objects.sphere = object_import("../res/models/primitives/sphere.obj", 0);
    objects.cylinder = object_import("../res/models/primitives/cylinder.obj", 0);
    objects.torus = object_import("../res/models/primitives/torus.obj", 0);
    objects.cone = object_import("../res/models/primitives/cone.obj", 0);
    objects.sponza_wall = object_import("../res/models/sponza/sponza_wall.obj", 1);
    objects.sponza_floor = object_import("../res/models/sponza/sponza_floor.obj", 1);
    objects.sponza_wall.m.texture = images.test;
    objects.sponza_wall.m.textured = true;
}

void render_dump()
{
    object_deport(objects.cube);
    object_deport(objects.sphere);
    object_deport(objects.cylinder);
    object_deport(objects.torus);
    object_deport(objects.cone);
    object_deport(objects.sponza_wall);
    object_deport(objects.sponza_floor);
}

//Main render loop.
void render()
{
    scene_init();

    dir_light sun = {
        {1, 1, 1},
        {1, 1, 1},
        2
    };

    dir_light farts = {
        {-1, -1, -1},
        {.5, .5, 1},
        1
    };

    array_insert(scene.dir_lights, sun);
    array_insert(scene.dir_lights, farts);

    objects.sponza_wall.t.position.z = 0;
    object_draw(objects.sponza_wall);
    object_draw(objects.sponza_floor);
    objects.sphere.t.position.z = 3;
    objects.sphere.t.position.y = 5;
    object_draw(objects.sphere);

    scene_clip_volume();
    scene_render();
    scene_clear();
}