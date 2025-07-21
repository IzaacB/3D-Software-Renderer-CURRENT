#include "ctagss.h"
#include "raster.h"

static f32_array raster_lerp(f32 i0, f32 d0, f32 i1, f32 d1)
{
     f32_array line;
     array_init(line);

     f32 slope = 0;

     if(i1 != i0)
     {
          slope = (d1 - d0) / (i1 - i0);
     }

     for (u32 i = 0; i < (u32)(i1 - i0); i++)
     {
          array_insert(line, d0 + slope * i);
     }

     return line;
}

void raster_ppx(f32 x, f32 y, color c)
{
    x = ((f32)CANVAS_WIDTH / 2 + x);
    y = ((f32)CANVAS_HEIGHT / 2 - y);

    bool on_screen = x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT;

    if (on_screen)
    {
        u32 color_dword = (u8)(c.b * 255) << 16 | (u8)(c.g * 255) << 8 | (u8)(c.r * 255);
        state.surface[(u32)y * CANVAS_WIDTH + (u32)x] = color_dword;
    }
}

void raster_line(v2 p0, v2 p1, color c)
{
     if (p0.x == p1.x && p0.y == p1.y){
          raster_ppx(p0.x, p0.y, c);

     }else if(fabsf(p1.x - p0.x) >= fabsf(p1.y - p0.y))
     {
          if (p1.x < p0.x)
          {
               v2 t = p0;
               p0 = p1;
               p1 = t;
          }
          f32_array line = raster_lerp(p0.x, p0.y, p1.x, p1.y);
          for (u32 x = 0; x < line.used; x++)
          {
               raster_ppx(x + p0.x, line.vals[x], c);
          }
          array_clear(line);
     }
     else
     {
          if (p1.y < p0.y)
          {
               v2 t = p0;
               p0 = p1;
               p1 = t;
          }
          f32_array line  = raster_lerp(p0.y, p0.x, p1.y, p1.x);
          for (u32 y = 0; y < line.used; y++)
          {
               raster_ppx(line.vals[y], y + p0.y, c);
          }
          array_clear(line);
     }
}

void raster_triangle_wireframe(u32 i)
{
     v3 p0 = scene.projected.vals[scene.faces.vals[i].i0];
     v3 p1 = scene.projected.vals[scene.faces.vals[i].i1];
     v3 p2 = scene.projected.vals[scene.faces.vals[i].i2];

     v2 h0 = v3_to_v2(p0);
     v2 h1 = v3_to_v2(p1);
     v2 h2 = v3_to_v2(p2);
     
     color c = {1, 1, 1};
     raster_line(h0, h1, c);
     raster_line(h1, h2, c);
     raster_line(h2, h0, c);
}

void raster_triangle_solid(u32 i)
{
    v3 p0 = scene.projected.vals[scene.faces.vals[i].i0];
    v3 p1 = scene.projected.vals[scene.faces.vals[i].i1];
    v3 p2 = scene.projected.vals[scene.faces.vals[i].i2];

    color c = {.7, .7, .7};

    v3 t;
    if (p1.y < p0.y)
    {
        t = p0;
        p0 = p1;
        p1 = t;
    }

    if (p2.y < p0.y)
    {
        t = p0;
        p0 = p2;
        p2 = t;
    }

    if (p2.y < p1.y)
    {
        t = p1;
        p1 = p2;
        p2 = t;
    }

    f32_array x01 = raster_lerp(p0.y, p0.x, p1.y, p1.x);
    f32_array x12 = raster_lerp(p1.y, p1.x, p2.y, p2.x);
    f32_array x02 = raster_lerp(p0.y, p0.x, p2.y, p2.x);
    f32_array x012; array_init(x012);
    array_concat(f32, x012, x01, x12);

    for (u32 y_index = 0; y_index < x012.used; y_index++)
    {
        i32 y = y_index + p0.y;

        i32 x_start = x02.vals[y_index];
        i32 x_end = x012.vals[y_index];

        if (x_start > x_end)
        {
            i32 t = x_start;
            x_start = x_end;
            x_end = t;
        }

        for (i32 x = x_start; x < x_end; x++)
        {
            raster_ppx(x, y, c);
        }
    }

    array_clear(x01);
    array_clear(x12);
    array_clear(x02);
    array_clear(x012);
}