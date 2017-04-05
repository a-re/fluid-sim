//
// Created by Restifo, Alexander (NIH/NCI) [F] on 3/29/17.
//

#include <SDL2/SDL.h>
#include "fluid_grid.h"

using namespace FluidSim;

FluidGrid::FluidGrid(SDL_Surface *s, SDL_Renderer *r, SDL_Texture *t) : surface(s), renderer(r), texture(t) {
    size = (GRID_SIZE + 2) * (GRID_SIZE + 2); // the +2's are there to have a boundary
    // there's definitely a better way to do this but I suck at c++
    y_velo = new SFLOAT[size];
    x_velo = new SFLOAT[size];
    y_velo_prev = new SFLOAT[size];
    x_velo_prev = new SFLOAT[size];

    density = new SFLOAT[size];
    density_prev = new SFLOAT[size];
}

FluidGrid::~FluidGrid() {
    delete[] x_velo;
    delete[] y_velo;
    delete[] y_velo_prev;
    delete[] x_velo_prev;
    delete[] density;
    delete[] density_prev;
}

void FluidGrid::run_simulation() {
    bool running = true;
    SDL_Event event;
    while (running) {
        const unsigned long start = SDL_GetPerformanceCounter();

        // Poll for events (adding densities, etc)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                running = false;
                break;
            }
        }

        unsigned *pixels = (unsigned *) surface->pixels;
        for (int i = 0; i < WND_SIZE; i++) {
            for (int j = 0; j < WND_SIZE; j++) {

            }
        }
        SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}

void FluidGrid::add_source(SFLOAT *dens, SFLOAT *src, SFLOAT dt) {
    for (int i = 0; i < size; i++) {
        dens[i] += dt * src[i];
    }
}

void FluidGrid::diffuse(int b, SFLOAT *dens, SFLOAT *dens_prev, SFLOAT diff, SFLOAT dt) {
    SFLOAT a = dt * diff * GRID_SIZE * GRID_SIZE;

    // Use Gauss-Seidel relaxation to solve the system of equations that is the density
    // diffs between grid cells
    // 20 iterations is good enough to have the fluid look good, but not
    // good for actual physical accuracy
    // For vars i and j, the offset is one on each side to account for boundary conditions
    for (int k = 0; k < 20; k++) {
        for (int i = 1; i <= GRID_SIZE; i++) {
            for (int j = 1; j <= GRID_SIZE; j++) {
                // Diffuse from all neighbors
                dens[IDX(i, j)] = (dens_prev[IDX(i, j)] + a * (dens[IDX(i - 1, j)] + dens[IDX(i + 1, j)] +
                                                               dens[IDX(i, j - 1)] + dens[IDX(i, j + 1)])) /
                                  (1 + 4 * a);
            }
        }
        set_bndy(b, dens);
    }
}

void FluidGrid::advect(int b, SFLOAT *dens, SFLOAT *dens_prev, SFLOAT *x_v, SFLOAT *y_v, SFLOAT dt) {
    SFLOAT dt0 = dt * GRID_SIZE;
    SFLOAT x, y, s0, s1, t0, t1;
    int x_corr, y_corr, x_corr_i, y_corr_i;
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            x = i - dt0 * x_v[IDX(i, j)];
            y = i - dt0 * y_v[IDX(i, j)];

            if (x < 0.5) x = 0.5;
            if (y < 0.5) y = 0.5;
            if (x > GRID_SIZE + 0.5) x = GRID_SIZE + 0.5;
            if (y > GRID_SIZE + 0.5) y = GRID_SIZE + 0.5;

            // Casting to int is just flooring the num
            x_corr = (int) x;
            y_corr = (int) y;
            x_corr_i = x_corr++;
            y_corr_i = y_corr++;

            s1 = x - x_corr;
            s0 = 1 - s1;
            t1 = y - y_corr;
            t0 = 1 - t1;

            // Finally set the density of each cell by linear interpolating the calculated
            // point into the grid cells around it
            dens[IDX(i, j)] = s0 * (t0 * dens_prev[IDX(x_corr, y_corr)] + t1 * dens_prev[IDX(x_corr, y_corr_i)]) +
                              s1 * (t0 * dens_prev[IDX(x_corr_i, y_corr)] + t1 * dens_prev[IDX(x_corr_i, y_corr_i)]);
        }
    }
    set_bndy(b, dens);
}

void FluidGrid::density_step(SFLOAT *dens, SFLOAT *dens_prev, SFLOAT *y_v, SFLOAT *x_v, SFLOAT diff, SFLOAT dt) {
    add_source(dens, dens_prev, dt);

    swap_array(dens, dens_prev);
    diffuse(0, dens, dens_prev, diff, dt);

    swap_array(dens, dens_prev);
    advect(0, dens, dens_prev, y_v, x_v, dt);
}

void FluidGrid::project(SFLOAT *y_v, SFLOAT *x_v, SFLOAT *y_v_prev, SFLOAT *x_v_prev) {
    SFLOAT h = 1.0 / GRID_SIZE;

    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            x_v_prev[IDX(i, j)] = -0.5 * h * (y_v[IDX(i + 1, j)] - y_v[IDX(i - 1, j)] +
                                              x_v[IDX(i, j + 1)] - x_v[IDX(i, j - 1)]);
            y_v_prev[IDX(i, j)] = 0;
        }
    }
    set_bndy(0, x_v_prev);
    set_bndy(0, y_v_prev);

    for (int k = 0; k < 20; k++) {
        for (int i = 1; i <= GRID_SIZE; i++) {
            for (int j = 1; j <= GRID_SIZE; j++) {
                y_v_prev[IDX(i, j)] = (x_v_prev[IDX(i, j)] + y_v_prev[IDX(i - 1, j)] + y_v_prev[IDX(i + 1, j)] +
                                       y_v_prev[IDX(i, j - 1)] + y_v_prev[IDX(i, j + 1)]) / 4;
            }
        }
        set_bndy(0, y_v_prev);
    }

    // Update the velocities
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            y_v[IDX(i, j)] -= 0.5 * (y_v_prev[IDX(i + 1, j)] - y_v_prev[IDX(i - 1, j)]) / h;
            x_v[IDX(i, j)] -= 0.5 * (y_v_prev[IDX(i, j + 1)] - y_v_prev[IDX(i, j - 1)]) / h;
        }
    }
    set_bndy(1, y_v);
    set_bndy(2, x_v);
}


void FluidGrid::set_bndy(int b_idx, SFLOAT *dens) {
    for (int i = 1; i <= GRID_SIZE; i++) {
        dens[IDX(0, i)] = b_idx == 1 ? -dens[IDX(1, i)] : dens[IDX(1, i)];
        dens[IDX(GRID_SIZE + 1, i)] = b_idx == 1 ? -dens[IDX(GRID_SIZE, i)] : dens[IDX(GRID_SIZE, i)];
        dens[IDX(i, 0)] = b_idx == 2 ? -dens[IDX(i, 1)] : dens[IDX(i, 1)];
        dens[IDX(i, GRID_SIZE + 1)] = b_idx == 2 ? -dens[IDX(i, GRID_SIZE)] : dens[IDX(i, GRID_SIZE)];
    }
    dens[IDX(0, 0)] = 0.5 * (dens[IDX(1, 0)] + dens[IDX(0, 1)]);
    dens[IDX(0, GRID_SIZE + 1)] = 0.5 * (dens[IDX(1, GRID_SIZE + 1)] + dens[IDX(0, GRID_SIZE)]);
    dens[IDX(GRID_SIZE + 1, 0)] = 0.5 * (dens[IDX(GRID_SIZE, 0)] + dens[IDX(GRID_SIZE + 1, 1)]);
    dens[IDX(GRID_SIZE + 1, GRID_SIZE + 1)] =
            0.5 * (dens[IDX(GRID_SIZE, GRID_SIZE + 1)] + dens[IDX(GRID_SIZE + 1, GRID_SIZE)]);
}

void FluidGrid::vel_step(SFLOAT *y_v, SFLOAT *x_v, SFLOAT *y_v_prev, SFLOAT *x_v_prev, SFLOAT dt) {
    add_source(y_v, y_v_prev, dt);
    add_source(x_v, x_v_prev, dt);

    swap_array(y_v_prev, y_v);
    diffuse(1, y_v, y_v_prev, VISCOCITY, dt);
    swap_array(x_v_prev, x_v);
    diffuse(2, x_v, x_v_prev, VISCOCITY, dt);

    project(y_v, x_v, y_v_prev, x_v_prev);
    swap_array(y_v_prev, y_v);
    swap_array(x_v_prev, x_v);

    advect(1, y_v, y_v_prev, y_v_prev, x_v_prev, dt);
    advect(2, x_v, x_v_prev, y_v_prev, x_v_prev, dt);
    project(y_v, x_v, y_v_prev, x_v_prev);
}

// Need this to be highly optimized so inline it
inline void FluidGrid::swap_array(SFLOAT *a0, SFLOAT *a) {
    SFLOAT *tmp = a0;
    a0 = a;
    a = tmp;
}