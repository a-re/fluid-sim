//
// Created by Restifo, Alexander (NIH/NCI) [F] on 3/29/17.
//

#include "fluid_grid.h"

class FluidGrid {
public:
    SFLOAT y_velo[size];
    SFLOAT x_velo[size];
    SFLOAT y_velo_prev[size];
    SFLOAT x_velo_prev[size];

    SFLOAT density[size];
    SFLOAT density_prev[size];

    FluidGrid::FluidGrid(SDL_Surface *s, int height, int width) :
            size((height + 2) * (width + 2)), // the +2's are there to have a boundary
            surface(s) {
        // do stuff
    }

    void FluidGrid::step() {
        // TESTING CODE
        unsigned *pixels = (unsigned *) surface->pixels;
        for (int i = 0; i < WND_HEIGHT; i++) {
            for (int j = 0; j < WND_WIDTH; j++) {
                pixels[i * WND_HEIGHT + j] = SDL_MapRGBA(surface->format, rand() * 255, rand() * 255, rand() * 255,
                                                         0xFF);
            }
        }
    }
};