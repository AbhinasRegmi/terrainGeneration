#include "fractal.h"

Fractal::Fractal(int height, int width, int octaves, float amplitude, float randomness, int scale): 
Noise(randomness),
octaves(octaves),
randomness(randomness),
height(height),
width(width),
amplitude(amplitude),
scale(scale){

}

float Fractal::genHeight(int x, int z){

    float X = (float) x / height;
    float Z = (float) z / width;

    float rand = exp2( -randomness );
    float frequency = 1.0f;
    float amp = amplitude;

    float result = 0.0f;

    for( int i = 0; i < octaves; i++){

        result += amp * (float)eval( frequency * X, frequency * Z);
        frequency *= 2.0f;
        amp *= rand;
    }


    return scale * result;
}