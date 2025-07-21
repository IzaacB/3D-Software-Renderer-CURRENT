#ifndef RASTER_H
#define RASTER_H

extern void raster_ppx(f32 x, f32 y, color c);
extern void raster_line(v2 p0, v2 p1, color c);
extern void raster_triangle_wireframe(v2 p0, v2 p1, v2 p2, color c);
extern void raster_triangle_solid(u32 i);

#endif