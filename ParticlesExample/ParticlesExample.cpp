#include "ResourcePath.h"

#include <iostream>
#include <SDL.h>

const int ScreenWidth = 1024;
const int ScreenHeight = 768;

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "Resource Path: " << getResourcePath() << std::endl;

    SDL_Window* pWindow = SDL_CreateWindow("Particles Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
    if (pWindow == nullptr)
    {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 2;
    }

    SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (pRenderer == nullptr)
    {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 3;
    }

    SDL_Event event;

    SDL_Rect particleRect{ 50,50,20,20 };
    bool shouldRun = true;
    while (shouldRun)
    {
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
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);
        SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(pRenderer, &particleRect);
        SDL_RenderPresent(pRenderer);
    }

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}

