#include "raster.h"

void raster_ppx(f32 x, f32 y, color c)
{
    x = (f32)CANVAS_WIDTH / 2 + x;
    y = (f32)CANVAS_HEIGHT / 2 - y;

    if (x - floor(x) >= .5)
    {
        x = ceil(x);
    }else
    {
        x = floor(x);
    }

    if (y - floor(x) >= .5)
    {
        y = ceil(y);
    }else
    {
        y = floor(y);
    }

    bool on_screen = x >= 0 || x < CANVAS_WIDTH || y >= 0 || y < CANVAS_HEIGHT;

    if (on_screen)
    {
        u32 color_dword = (u8)(c.b * 255) << 16 | (u8)(c.g * 255) << 8 | (u8)(c.r * 255);
        state.surface[(u32)y * CANVAS_WIDTH + (u32)x] = color_dword;
    }
}