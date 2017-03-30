//
// Created by Restifo, Alexander (NIH/NCI) [F] on 3/30/17.
//

#ifndef FLUID_SIM_FLUID_H
#define FLUID_SIM_FLUID_H

// Global macros
#define SFLOAT double // so we can easily change the precision of our calculations
#define IDX(i, j) (i + (N + 2) * j) // converts an (x, y) coordinate into the proper index of a 1d array

int main(int argc, char *argv[]);

#endif //FLUID_SIM_FLUID_H
