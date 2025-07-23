#include "ctagss.h"
#include "render.h"
#include "viewport.h"

struct State state;
struct Scene scene;
struct Images images;
struct Objects objects;
struct Viewport viewport;

int main(int argc, char *argv[])
{
    render_init();
    render_preload();
    viewport_init();

    f32 previous = SDL_GetTicks();
    state.delta = 0;

    bool running = true;
    while (running)
    {
        f32 current = SDL_GetTicks();
        state.delta = (current - previous) / 1000;
        previous = current;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        state.keystate = SDL_GetKeyboardState(NULL);

        viewport_control();

        render_clear();
        render();
        render_update();

        u32 frame_time = SDL_GetTicks() - current;
        if (frame_time < (1000 / TARGET_FPS))
        {
            SDL_Delay((1000 / TARGET_FPS) - frame_time);
        }
    }

    render_dump();
    
    render_quit();
}