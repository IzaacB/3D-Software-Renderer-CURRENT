#include "ctagss.h"
#include "scene.h"

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