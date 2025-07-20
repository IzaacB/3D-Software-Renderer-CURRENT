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

}