//
// Created by Restifo, Alexander (NIH/NCI) [F] on 3/29/17.
//

#ifndef FLUID_SIM_FLUID_GRID_H
#define FLUID_SIM_FLUID_GRID_H

#include "fluid.h"

namespace FluidSim {
    class FluidGrid {
    public:
        // TODO: Make this 3d (what z_velo and z_velo_prev are for)
        SFLOAT *x_velo;
        SFLOAT *y_velo;
        SFLOAT *x_velo_prev;
        SFLOAT *y_velo_prev;

        SFLOAT *density;
        SFLOAT *density_prev;

        FluidGrid(SDL_Surface *s, SDL_Renderer *r, SDL_Texture *t);

        ~FluidGrid();

        SDL_Surface *surface;
        SDL_Renderer *renderer;
        SDL_Texture *texture;

        // Density solver methods
        void run_simulation();

    private:
        int size;
        const SFLOAT VISCOCITY = 0.1;

        // ----------Fluid simulation functions----------
        // Density field solver methods
        void add_source(SFLOAT *dens, SFLOAT *src, SFLOAT dt);

        void diffuse(int b, SFLOAT *dens, SFLOAT *dens_prev, SFLOAT diff, SFLOAT dt);

        void advect(int b, SFLOAT *dens, SFLOAT *dens_prev, SFLOAT *y_v, SFLOAT *x_v, SFLOAT dt);

        void density_step(SFLOAT *dens, SFLOAT *dens_prev, SFLOAT *y_v, SFLOAT *x_v, SFLOAT diff, SFLOAT dt);

        // Velocity field solver functions
        void project(SFLOAT *y_v, SFLOAT *x_v, SFLOAT *y_v_prev, SFLOAT *x_v_prev);

        void vel_step(SFLOAT *y_v, SFLOAT *x_v, SFLOAT *y_v_prev, SFLOAT *x_v_prev, SFLOAT dt);

        // Function to handle boundary conditions
        void set_bndy(int b_idx, SFLOAT *dens);
        // ----------End fluid simulation functions----------

        // Utility functions
        void generate_color_palette();

        void swap_array(SFLOAT *a0, SFLOAT *a);
    };
}
#endif //FLUID_SIM_FLUID_GRID_H
