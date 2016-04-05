#include "ResourcePath.h"

#include "Particle.h"
#include "ParticleColored.h"
#include "ParticleEmitter.h"

#include <iostream>
#include <SDL.h>

const int ScreenWidth = 1024;
const int ScreenHeight = 768;

SDL_Window* pWindow = nullptr;
SDL_Renderer* pRenderer = nullptr;

int sdl_create()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "Resource Path: " << getResourcePath() << std::endl;

    pWindow = SDL_CreateWindow("Particles Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
    if (pWindow == nullptr)
    {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 2;
    }

    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (pRenderer == nullptr)
    {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 3;
    }

    return 0;
}

void sdl_destroy()
{
    if (pRenderer != nullptr) {
        SDL_DestroyRenderer(pRenderer);
        pRenderer = nullptr;
    }

    if (pWindow != nullptr) {
        SDL_DestroyWindow(pWindow);
        pWindow = nullptr;
    }

    SDL_Quit();
}

bool sdl_handle_input()
{
    bool shouldRun = true;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            shouldRun = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            shouldRun = event.key.keysym.sym != SDLK_ESCAPE;
        }
    }

    return shouldRun;
}

int main(int argc, char** argv)
{
    int ret = sdl_create();
    if (ret != 0) return ret;

    ParticleEmitter<Particle> emitter({ 10, ScreenHeight / 3 });
    ParticleEmitter<ParticleColored> emitterColored({ 10, ScreenHeight-(ScreenHeight / 3) });

    bool shouldRun = true;
    Uint32 lastFrameTime = SDL_GetTicks();

    while (shouldRun)
    {
        shouldRun = sdl_handle_input();

        // Calculate delta time
        Uint32 nowTime = SDL_GetTicks(); // in ms
        float deltaTime = (nowTime - lastFrameTime) / 1000.f; // in secs

        // Update all the particles
        emitter.Update(deltaTime);
        emitterColored.Update( deltaTime );

        lastFrameTime = nowTime;

        // Clear to black
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        // Render all the particles
        emitter.Render(pRenderer);
        emitterColored.Render(pRenderer);

        SDL_RenderPresent(pRenderer);

        SDL_Delay(1000/30); // 30 fps
    }

    sdl_destroy();
    return 0;
}
