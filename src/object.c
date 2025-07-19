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
    
    return o;
}

void object_deport(object o)
{
    array_clear(o.vertices);
    array_clear(o.faces);
}

void object_draw(object o)
{
    //todo
}