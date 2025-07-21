#include "ctagss.h"
#include "object.h"

object object_import(char *path)
{
    object o = 
    {
        .t = {
            {0, 0, 0},
            {0, 0, 0},
            {1, 1, 1}
        }
    };

    array_init(o.vertices);
    array_init(o.faces);
    array_init(o.normals);

    FILE *file = fopen(path, "r");
    char string[256];

    while(fgets(string, sizeof(string), file))
    {
        char type = string[0];

        switch (type)
        {
            case 'v':
                f32 x, y, z;
                sscanf(string + 2, "%f %f %f", &x, &y, &z);
                v3 v = {x, y, z};
                array_insert(o.vertices, v);
                break;

            case 'f':
                u32 i0, i1, i2;
                sscanf(string + 2, "%u %u %u", &i0, &i1, &i2);
                face f = {i0 - 1, i1 - 1, i2 - 1};
                array_insert(o.faces, f);
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
}

void object_draw(object o)
{
    u32 vertice_offset = scene.vertices.used;
    for (u32 i = 0; i < o.vertices.used; i++)
    {
        v3 v = o.vertices.vals[i];
        v = v3_transform(v, o.t, 0);
        v = v3_transform(v, viewport.t, 1);
        array_insert(scene.vertices, v);
    }

    for (u32 i = 0; i < o.faces.used; i++)
    {
        face f = o.faces.vals[i];
        f.i0 += vertice_offset;
        f.i1 += vertice_offset;
        f.i2 += vertice_offset;

        v3 p0 = scene.vertices.vals[f.i0];
        v3 p1 = scene.vertices.vals[f.i1];
        v3 p2 = scene.vertices.vals[f.i2];
        
        v3 origin = {
            (p0.x + p1.x + p2.x) / 3,
            (p0.y + p1.y + p2.y) / 3,
            (p0.z + p1.z + p2.z) / 3
        };

        v3 normal = v3_transform(v3_transform(o.normals.vals[i], o.t, 2), viewport.t, 3);

        if (v3_dot(v3_norm(origin), normal) < 0)
        {
            array_insert(scene.faces, f);
            array_insert(scene.normals, normal);
        }
    }
}