#ifndef RASTER_H
#define RASTER_H

extern void raster_ppx(f32 x, f32 y, color c);
extern void raster_ppx_z(f32 x, f32 y, f32 z, color c);
extern void raster_line(v2 p0, v2 p1, color c);
extern void raster_triangle_wireframe(u32 i);
extern void raster_triangle_solid(u32 i);
extern void raster_triangle_textured(u32 i);
extern void raster_sprite3D(image sprite, v3 pos, f32 size);

#endif