#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "fluid_grid.h"

using namespace FluidSim;

int main(int argc, char *argv[]) {
    // Firstly, init everything
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::cerr << "SDL_Init() failed" << std::endl;

        SDL_Quit();
        return 1;
    }

    // Create our game screen
    SDL_Window *screen = SDL_CreateWindow("Fluid simulation", 100, 100, WND_SIZE, WND_SIZE, SDL_WINDOW_SHOWN);

    // Create our renderer for the fluid sim
    // Third param requests GPU acceleration and vsync
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Get the surface our texture will be drawn on
    SDL_Surface *surface = SDL_CreateRGBSurface(0, WND_SIZE, WND_SIZE, WND_BPP, 0, 0, 0, 0);

    // Create our texture we will draw the pixels on
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);

    // Create our fluid grid
    std::unique_ptr<FluidGrid> fluid(new FluidGrid(surface, renderer, tex));

    // Main drawing loop
    fluid->run_simulation();

    // Destroy SDL resources
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(tex);

    SDL_Quit();
    return 0;
}