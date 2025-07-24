#include "ctagss.h"
#include "image.h"
#include "raster.h"

image image_import(const char *path)
{
    image img;

    SDL_Surface *loaded = SDL_LoadBMP(path);
    img.source = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(loaded);

    img.pixels = img.source->pixels;
    img.width = img.source->w;
    img.height = img.source->h;

    img.h_frames = 1;
    img.v_frames = 1;

    return img;
}

void image_deport(image img)
{
    SDL_FreeSurface(img.source);
    img.source = NULL;
    img.pixels = NULL;
    img.width = 0;
    img.height = 0;
}

color image_sample(image img, i32 x, i32 y)
{
    u8 r, g, b, a;
    u32 pixel = img.pixels[y * img.source->pitch / 4 + x];
    SDL_GetRGBA(pixel, img.source->format, &r, &g, &b, &a);
    color col = {(f32)r / 255, (f32)g / 255, (f32)b / 255, (f32)a / 255};
    return col;
}

void image_draw(image img, i32 x, i32 y)
{
    for (i32 sy = 0; sy < img.height; sy++)
    {
        for (i32 sx = 0; sx < img.width; sx++)
        {
            color col = image_sample(img, sx, sy);
            if (col.a == 1)
            {
                raster_ppx(x + sx, y - sy, col);
            }
        }
    }
}

void image_draw_frame(image img, i32 x, i32 y, u32 frame)
{
    u32 frame_width = img.width / img.h_frames;
    u32 frame_height = img.height / img.v_frames;

    for (i32 sy = 0; sy < frame_height; sy++)
    {
        for (i32 sx = 0; sx < frame_width; sx++)
        {
            color col = image_sample(img, sx + ((frame % img.h_frames) * frame_width), sy + ((frame / img.h_frames) * frame_height));
            if (col.a == 1)
            {
                raster_ppx(x + sx, y - sy, col);
            }
        }
    }
}