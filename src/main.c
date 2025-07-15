#include "ctagss.h"
#include "render.h"

struct State state;

int main(int argc, char *argv[])
{
    render_init();

    bool running = true;
    while (running)
    {
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
        render_clear();
        render();
        render_update();
    }

    render_quit();
}