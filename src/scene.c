#include "ctagss.h"
#include "scene.h"
#include "raster.h"

void scene_init()
{
    array_init(scene.vertices);
    array_init(scene.faces);
}

void scene_clear()
{
    array_clear(scene.vertices);
    array_clear(scene.faces);
}

void scene_render()
{
    v3_array projected;
    array_init(projected);

    for (u32 i = 0; i < scene.vertices.used; i++)
    {
        v3 vert = scene.vertices.vals[i];
        v3 point = {
            round((vert.x / vert.z * viewport.t.scale.z) * (CANVAS_WIDTH / viewport.t.scale.x)),
            round((vert.y / vert.z * viewport.t.scale.z) * (CANVAS_HEIGHT / viewport.t.scale.y)),
            vert.z
        };

        array_insert(projected, point);
    }

    for (u32 i = 0; i < scene.faces.used; i++)
    {
        color c = {1, 1, 1};
        
        v3 p0 = projected.vals[scene.faces.vals[i].i0];
        v3 p1 = projected.vals[scene.faces.vals[i].i1];
        v3 p2 = projected.vals[scene.faces.vals[i].i2];

        raster_triangle_wireframe(v3_to_v2(p0), v3_to_v2(p1), v3_to_v2(p2), c);
    }

    array_clear(projected);
}