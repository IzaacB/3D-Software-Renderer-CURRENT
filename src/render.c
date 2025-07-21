#include "ctagss.h"
#include "render.h"
#include "scene.h"
#include "raster.h"
#include "object.h"

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
    objects.cube = object_import("../res/models/cube.obj");
}

void render_dump()
{
    object_deport(objects.cube);
}

//Main render loop.
void render()
{
    scene_init();
    //viewport.t.position.x += .1 * state.delta;
    objects.cube.t.position.z = 5;
    objects.cube.t.rotation.x += 1 * state.delta;
    objects.cube.t.rotation.y += 1 * state.delta;
    objects.cube.t.rotation.z += 1 * state.delta;
    object_draw(objects.cube);
    objects.cube.t.position.z = 10;
    object_draw(objects.cube);
    scene_render();
    scene_clear();
}