#include "ctagss.h"
#include "render.h"
#include "scene.h"
#include "raster.h"
#include "object.h"
#include "image.h"
#include "text.h"

void render_settings()
{
    settings.color_range_red = 16;
    settings.color_range_green = 16;
    settings.color_range_blue = 16;

    settings.render_distance = 25;
    settings.textured = true;
    settings.wireframe = false;

    color ambient_light = {.25, .25, .25};
    settings.ambient_light = ambient_light;

    settings.fog = true;
    settings.fog_intensity = -10;
    color fog_color = {1, 1, 1};
    settings.fog_color = fog_color;

    settings.show_stats = true;

    settings.mouse_look = false;
}

void render_init()
{
    state.window = SDL_CreateWindow(
        "Software Rasterizer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 800,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    state.renderer = SDL_CreateRenderer(
        state.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    state.canvas = SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        CANVAS_WIDTH, CANVAS_HEIGHT);

    stats.frequency = 0;
    state.key_timer = 0;
}

static void render_update_stats()
{
    if (stats.frequency <= 0)
    {
        stats.fps = floor(1 / state.delta);
        stats.frequency = .5;
    }else
    {
        stats.frequency -= 1 * state.delta;
    }
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
    images.busts = image_import("../res/images/busts.bmp");
    images.ceiling = image_import("../res/images/ceiling.bmp");
    images.floor = image_import("../res/images/floor.bmp");
    images.wall = image_import("../res/images/wall.bmp");

    objects.busts = object_import("../res/models/busts.obj", 1);
    objects.busts.m.texture = images.busts;
    objects.busts.m.textured = true;

    objects.ceiling = object_import("../res/models/ceiling.obj", 1);
    objects.ceiling.m.texture = images.ceiling;
    objects.ceiling.m.textured = true;

    objects.floor = object_import("../res/models/floor.obj", 1);
    objects.floor.m.texture = images.floor;
    objects.floor.m.textured = true;

    objects.frames = object_import("../res/models/frames.obj", 1);
    color gold = {.5, .4, .12};
    objects.frames.m.c = gold;

    objects.lights = object_import("../res/models/lights.obj", 1);
    color white = {1, 1, 1};
    objects.lights.m.c = white;

    objects.wall = object_import("../res/models/wall.obj", 1);
    objects.wall.m.texture = images.wall;
    objects.wall.m.textured = true;
}

void render_dump()
{
    image_deport(images.busts);
    image_deport(images.ceiling);
    image_deport(images.floor);
    image_deport(images.wall);

    object_deport(objects.busts);
    object_deport(objects.ceiling);
    object_deport(objects.floor);
    object_deport(objects.frames);
    object_deport(objects.lights);
    object_deport(objects.wall);
}

// Main render loop.
void render()
{
    scene_init();

    point_light l0 = {
        {-2, 2.8, 0},
        {1, 1, 1},
        1,
        10
    };

    point_light l1 = {
        {0, 2.8, -2},
        {1, 1, 1},
        1,
        10
    };

    point_light l2 = {
        {2, 2.8, 0},
        {1, 1, 1},
        1,
        10
    };

    dir_light sun = {
        {-1, -1, -1},
        {.5, .5, 1},
        .5
    };

    array_insert(scene.point_lights, l0);
    array_insert(scene.point_lights, l1);
    array_insert(scene.point_lights, l2);
    array_insert(scene.dir_lights, sun);

    object_draw(objects.busts);
    object_draw(objects.ceiling);
    object_draw(objects.floor);
    object_draw(objects.frames);
    object_draw(objects.lights);
    object_draw(objects.wall);

    scene_clip_volume();
    scene_render();

    stats.rendered_faces = scene.faces.used;
    stats.point_lights = scene.point_lights.used;
    stats.dir_lights = scene.dir_lights.used;

    render_update_stats();

    text_render();

    scene_clear();
    stats.total_faces = 0;
}