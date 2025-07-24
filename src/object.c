#include "ctagss.h"
#include "object.h"

object object_import(char *path, u8 mode)
{
    object o =
        {
            .t = {
                {0, 0, 0},
                {0, 0, 0},
                {1, 1, 1}},
            .m = {.c = {.75, .75, .75}, .textured = false}};

    array_init(v3, o.vertices, 1);
    array_init(face, o.faces, 1);
    array_init(v3, o.normals, 1);
    array_init(v2, o.uvs, 1);

    FILE *file = fopen(path, "r");
    char string[256];

    while (fgets(string, sizeof(string), file))
    {
        char type = string[0];

        switch (type)
        {
        case 'v':
            if (string[1] == ' ')
            {
                f32 x, y, z;
                sscanf(string + 2, "%f %f %f", &x, &y, &z);
                v3 v = {x, y, z};
                array_insert(o.vertices, v);
            }
            else if (string[1] == 't')
            {
                f32 u, v;
                sscanf(string + 3, "%f %f", &u, &v);
                v2 uv = {u, v};
                array_insert(o.uvs, uv);
            }

            break;

        case 'f':
            if (mode == 0)
            {
                u32 i0, i1, i2;
                sscanf(string + 2, "%u %u %u", &i0, &i1, &i2);
                face f = {i0 - 1, i1 - 1, i2 - 1};
                array_insert(o.faces, f);
            }
            else if (mode == 1)
            {
                u32 i0, i1, i2;
                u32 uv0, uv1, uv2;
                sscanf(string + 2, "%u/%u %u/%u %u/%u", &i0, &uv0, &i1, &uv1, &i2, &uv2);
                face f = {i0 - 1, i1 - 1, i2 - 1, uv0 - 1, uv1 - 1, uv2 - 1};
                array_insert(o.faces, f);
            }

            break;
        }
    }

    fclose(file);

    for (u32 i = 0; i < o.faces.used; i++)
    {
        v3 p0 = o.vertices.vals[o.faces.vals[i].i0];
        v3 p1 = o.vertices.vals[o.faces.vals[i].i1];
        v3 p2 = o.vertices.vals[o.faces.vals[i].i2];

        v3 s0 = v3_sub(p1, p0);
        v3 s1 = v3_sub(p2, p0);

        v3 normal = v3_cross(s0, s1);

        normal = v3_norm(normal);

        array_insert(o.normals, normal);
    }

    return o;
}

void object_deport(object o)
{
    array_clear(o.vertices);
    array_clear(o.faces);
    array_clear(o.normals);
    array_clear(o.uvs);
}

void object_draw(object o)
{
    u32 vertice_offset = scene.vertices.used;
    u32 uv_offset = scene.uvs.used;

    for (u32 i = 0; i < o.vertices.used; i++)
    {
        v3 v = o.vertices.vals[i];
        v = v3_transform(v, o.t, 0);
        v = v3_transform(v, viewport.t, 1);
        array_insert(scene.vertices, v);
    }

    for (u32 i = 0; i < o.uvs.used; i++)
    {
        array_insert(scene.uvs, o.uvs.vals[i]);
    }

    for (u32 i = 0; i < o.faces.used; i++)
    {
        face f = o.faces.vals[i];
        f.i0 += vertice_offset;
        f.i1 += vertice_offset;
        f.i2 += vertice_offset;

        f.uv0 += uv_offset;
        f.uv1 += uv_offset;
        f.uv2 += uv_offset;

        v3 p0 = scene.vertices.vals[f.i0];
        v3 p1 = scene.vertices.vals[f.i1];
        v3 p2 = scene.vertices.vals[f.i2];

        v3 origin = {
            (p0.x + p1.x + p2.x) / 3,
            (p0.y + p1.y + p2.y) / 3,
            (p0.z + p1.z + p2.z) / 3};

        v3 normal = v3_transform(v3_transform(o.normals.vals[i], o.t, 2), viewport.t, 3);

        if (v3_dot(v3_norm(origin), normal) < 0)
        {
            array_insert(scene.faces, f);
            array_insert(scene.normals, normal);

            array_insert(scene.material_indices, scene.materials.used);
        }
    }
    array_insert(scene.materials, o.m);
}