//
// Created by Restifo, Alexander (NIH/NCI) [F] on 3/29/17.
//

#ifndef FLUID_SIM_FLUID_GRID_H
#define FLUID_SIM_FLUID_GRID_H

class FluidGrid {
public:
    FluidGrid(SDL_Surface *s, int height, int width);

    ~FluidGrid();

    int size;
    SDL_Surface *surface;

    void step();
private:
    void generate_color_palette();
};

#endif //FLUID_SIM_FLUID_GRID_H
