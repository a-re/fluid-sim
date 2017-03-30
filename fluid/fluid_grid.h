//
// Created by Restifo, Alexander (NIH/NCI) [F] on 3/29/17.
//

#ifndef FLUID_SIM_FLUID_GRID_H
#define FLUID_SIM_FLUID_GRID_H

#define SFLOAT double // so we can easily change the precision of our calculations
#define IDX(i, j) ((i) + (N + 2) * j)) // calculate 1D array index from 2D coordinate. It is better to use 1D arrays for efficiency

class FluidGrid {
public:
    FluidGrid(int height, int width) :
            size((height + 2) * (width + 2)) // the +2's are there to have a boundary
    {}

    ~FluidGrid() {}

    int size;

    SFLOAT y_velo[size];
    SFLOAT x_velo[size];
    SFLOAT y_velo_prev[size];
    SFLOAT x_velo_prev[size];

    SFLOAT density[size];
    SFLOAT density_prev[size];

    SDL_Surface &surface;

    void draw();

private:
    //void generate_color_palette();
};


#endif //FLUID_SIM_FLUID_GRID_H
