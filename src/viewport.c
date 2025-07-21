#include "ctagss.h"
#include "viewport.h"

void viewport_init()
{
    transform t = {
        {0, 0, 0},
        {0, 0, 0},
        {(f32)CANVAS_WIDTH / (f32)CANVAS_HEIGHT, 1, 1}
    };

    viewport.t = t;

    plane near = 
    {
        {0, 0, 1},
        -viewport.t.scale.z
    };

    viewport.near = near;

    plane far = 
    {
        {0, 0, -1},
        50, //Change this later.
    };

    viewport.far = far;

    plane right = 
    {
        {-viewport.t.scale.z, 0, viewport.t.scale.x / 2},
        0
    };

    viewport.right = right;

    plane left = 
    {
        {viewport.t.scale.z, 0, viewport.t.scale.x / 2},
        0
    };

    viewport.left = left;

    plane top =
    {
        {0, -viewport.t.scale.z, viewport.t.scale.y / 2},
        0
    };

    viewport.top = top;

    plane bottom = 
    {
        {0, viewport.t.scale.z, viewport.t.scale.y / 2},
        0
    };

    viewport.bottom = bottom;
}