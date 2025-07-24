#include "ctagss.h"
#include "render.h"
#include "scene.h"
#include "raster.h"
#include "object.h"
#include "image.h"

void render_settings()
{
    settings.color_range = 16;

    settings.render_distance = 25;
    settings.wireframe = false;

    color ambient_light = {.5, .5, .5};
    settings.ambient_light = ambient_light;

    settings.fog = true;
    settings.fog_intensity = -.2;
    color fog_color = {1, 1, 1};
    settings.fog_color = fog_color;
}

void render_init()
{
    state.window = SDL_CreateWindow(
        "Software Rasterizer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 800,
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
    // Images.
    images.bricks = image_import("../res/images/bricks.bmp");
    images.grass = image_import("../res/images/grass.bmp");
    images.tile = image_import("../res/images/tile.bmp");
    images.yellow_wood = image_import("../res/images/yellow_wood.bmp");
    images.green = image_import("../res/images/green.bmp");

    // 3D primitives.
    objects.cube = object_import("../res/models/primitives/cube.obj", 0);
    objects.sphere = object_import("../res/models/primitives/sphere.obj", 0);
    objects.cylinder = object_import("../res/models/primitives/cylinder.obj", 0);
    objects.torus = object_import("../res/models/primitives/torus.obj", 0);
    objects.cone = object_import("../res/models/primitives/cone.obj", 0);

    // Scene objects.
    objects.scene_grass = object_import("../res/models/scene_grass.obj", 1);
    objects.scene_grass.m.texture = images.grass;
    objects.scene_grass.m.textured = true;

    objects.scene_ring = object_import("../res/models/scene_ring.obj", 1);
    objects.scene_ring.m.texture = images.yellow_wood;
    objects.scene_ring.m.textured = true;

    objects.scene_tile = object_import("../res/models/scene_tile.obj", 1);
    objects.scene_tile.m.texture = images.tile;
    objects.scene_tile.m.textured = true;

    objects.scene_wall = object_import("../res/models/scene_wall.obj", 1);
    objects.scene_wall.m.texture = images.bricks;
    objects.scene_wall.m.textured = true;
}

void render_dump()
{
    // Unload images.
    image_deport(images.bricks);
    image_deport(images.grass);
    image_deport(images.tile);
    image_deport(images.yellow_wood);
    image_deport(images.green);

    // Unload primitives.
    object_deport(objects.cube);
    object_deport(objects.sphere);
    object_deport(objects.cylinder);
    object_deport(objects.torus);
    object_deport(objects.cone);

    // Unload scene objects.
    object_deport(objects.scene_grass);
    object_deport(objects.scene_ring);
    object_deport(objects.scene_tile);
    object_deport(objects.scene_wall);
}

// Main render loop.
void render()
{
    scene_init();

    // Insert light sources.
    dir_light sun = {
        {1, -1, 0},
        {1, 1, 1},
        1};
    array_insert(scene.dir_lights, sun);

    // Draw scene.
    object_draw(objects.scene_grass);
    object_draw(objects.scene_ring);
    object_draw(objects.scene_tile);
    object_draw(objects.scene_wall);

    sprite3D green = 
    {
        images.green,
        {
            {0, 1.75, 0},
            {0, 0, 0},
            {1, 1, 1}
        }
    };

    scene_clip_volume();
    scene_render();

    raster_sprite3D(green);

    scene_clear();
}