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
        50, //TO-DO, Make a view distance setting and swap with this value.
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

void viewport_control()
{
    f32 move_speed = 5;
    f32 rotation_speed = 1;

    if (state.keystate[SDL_SCANCODE_W])
    {
        viewport.t.position.z += move_speed * cos(viewport.t.rotation.y) * state.delta;
        viewport.t.position.x += move_speed * sin(viewport.t.rotation.y) * state.delta;
    }

    if (state.keystate[SDL_SCANCODE_S])
    {
        viewport.t.position.z -= move_speed * cos(viewport.t.rotation.y) * state.delta;
        viewport.t.position.x -= move_speed * sin(viewport.t.rotation.y) * state.delta;
    }

    if (state.keystate[SDL_SCANCODE_A])
    {
        viewport.t.position.x -= move_speed * cos(viewport.t.rotation.y) * state.delta;
        viewport.t.position.z += move_speed * sin(viewport.t.rotation.y) * state.delta;
    }

    if (state.keystate[SDL_SCANCODE_D])
    {
        viewport.t.position.x += move_speed * cos(viewport.t.rotation.y) * state.delta;
        viewport.t.position.z -= move_speed * sin(viewport.t.rotation.y) * state.delta;
    }

    if (state.keystate[SDL_SCANCODE_RIGHT])
    {
        viewport.t.rotation.y += rotation_speed * state.delta;
    }

    if (state.keystate[SDL_SCANCODE_LEFT])
    {
        viewport.t.rotation.y -= rotation_speed * state.delta;
    }

    if (state.keystate[SDL_SCANCODE_DOWN])
    {
        viewport.t.rotation.x += rotation_speed * state.delta;
    }

    if (state.keystate[SDL_SCANCODE_UP])
    {
        viewport.t.rotation.x -= rotation_speed * state.delta;
    }
}