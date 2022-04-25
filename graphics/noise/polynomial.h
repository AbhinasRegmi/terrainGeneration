// #ifndef POLYNOMIAL_H
// #define POLYNOMIAL_H

// #define GridPerLen 1000/50

// #include <iostream>
// #include <vector>
// #include <cmath>


// struct constant{
//     float a, b, c, d;
// };

// typedef constant constant;

// static constant hi;
// static std::vector<std::vector<constant>> constDist(GridPerLen, std::vector<constant>(GridPerLen, hi));
// static std::vector<std::vector<float>> gridMaximums(GridPerLen, std::vector<float>(GridPerLen, 0));

// class Polynomial{

//     private:

//         //each gird will be normalized 

//         static int gridLength;
//         static int heightTerrain;
//         static int widthTerrain;
        
//         static int gridX;
//         static int gridY;



//     public:

//         //linear function
//         static float linear(int x, int z);

//         //setup grid
//         static void gridSetup(int gridLength, int height_terrain, int width_terrain);

//         //nonLinear
//         static float nonLinear(int x, int z);

//     private:

//         //S function
//         static float S(float input);

//         //determine value of a, b, c, d
//         static void determineConstant(int gridLength, int height, int width);

//         static void findMaxHeight();
// };
    
// #endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <cmath>
#include <vector>
#include <iostream>

struct gridVert{
    float a;
    float b;
    float c;
    float d;
};

typedef gridVert gridVert;

class Noise{

    public:
        Noise(int height, int width, int sqGridLen);

        float genNoise(int xCoord, int zCoord);

        float polynomial(int xCoord, int zCoord);
    private:
        int terrainHeight;
        int terrainWidth;
        int sqGridLen;

        unsigned int Xindex;
        unsigned int Zindex;

        //store four height points of a grid
        std::vector<std::vector<gridVert>> vert;

        void genGridPointsHeight();

        float poly(float in);


};
#endif