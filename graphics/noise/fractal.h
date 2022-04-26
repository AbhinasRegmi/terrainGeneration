#ifndef FRACTAL_H
#define FRACTAL_H

#include <cmath>

#include "simplex.h"

class Fractal : public Noise{

    public:
        Fractal(int height, int width, int octaves = 8, float amplitude = 2.0f,  float randomness = 0.9, int scale = 100);

        float genHeight(int x, int z);
    private:

        int height;
        int width;
        int octaves;
        float randomness;
        float amplitude;
        int scale;
};

#endif