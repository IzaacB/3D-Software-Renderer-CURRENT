#include "ctagss.h"
#include "render.h"

struct State state;
struct Scene scene;
struct Objects objects;

int main(int argc, char *argv[])
{
    render_init();
    render_preload();

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

    render_dump();
    
    render_quit();
}