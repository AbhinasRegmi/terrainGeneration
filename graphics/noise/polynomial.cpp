// #include "polynomial.h"

// //initialize the static variables

// int Polynomial::heightTerrain = 1;
// int Polynomial::widthTerrain = 1;

// int Polynomial::gridLength = 1;
// int Polynomial::gridX = 0;
// int Polynomial::gridY = 0;


// float Polynomial::nonLinear(int x, int z){
//     //find out in which grid coordinate the current vertex lies
    
//     //normalized coordinates
//     float normI = -(float) x / heightTerrain+ 2 * (float) x / heightTerrain;
//     float normJ = -(float) z / widthTerrain + 2 * (float) z / widthTerrain;

//     gridX = x / gridLength;
//     gridY = z / gridLength;

//     float normGridX = normI / gridLength;
//     float normGridY = normJ / gridLength;

//     float result = 
//     constDist[gridX][gridY].a + 
//     ( constDist[gridX][gridY].b - constDist[gridX][gridY].a ) * Polynomial::S( normI - normGridX) + 
//     ( constDist[gridX][gridY].c - constDist[gridX][gridY].a ) * Polynomial::S( normJ - normGridY) + 
//     ( constDist[gridX][gridY].a - constDist[gridX][gridY].b - constDist[gridX][gridY].c + constDist[gridX][gridY].d) * Polynomial::S( normI - normGridX) * Polynomial::S(normJ - normGridY);

//     return 3 * cos(result);
// }

// //define s
// float Polynomial::S(float input){

//     return ( 3 * input  * input - 2 * input * input * input);
// }

// //define determineConstant
// void Polynomial::gridSetup( int gridLength_in , int height, int width){

//     gridLength = gridLength_in;
//     heightTerrain = height;
//     widthTerrain = width;

//     float u, v;

//     for( int i = 0; i < height / gridLength_in; i++ ){

//         for( int j = 0; j < width / gridLength_in; j++ ){

//             u = 3 * i / 0.3415926535;
//             v = 3 * j / 0.3415926535;

//             constDist[i][j].a = 2 * u * v * ( u + v) - 1;

//             if( i == 0 || j == 0 || i == height / gridLength || j == width / gridLength){

//                 constDist[i][j].b = 0;
//                 constDist[i][j].c = 0;
//                 constDist[i][j].d = 0;

//             }else{

//                 constDist[i - 1][j].b = constDist[i][j].a;
//                 constDist[i][j - 1].c = constDist[i][j].a;
//                 constDist[i - 1][j - 1].d = constDist[i][j].a;
//             }

//         }
//     }
    
//     //find maximum height
//     findMaxHeight();
// }

// void Polynomial::findMaxHeight(){

//     int x = 0;
//     int y = 0;


//     float normI = - x / heightTerrain + 2 * x / heightTerrain;
//     float normJ = - y / widthTerrain + 2 * y / widthTerrain;

//     for( int i = 0; i < GridPerLen; i++){
//         for( int j = 0; j < GridPerLen; j++){

//             gridMaximums[i][j] = 
//                 constDist[i][j].a + 
//                 ( constDist[i][j].b - constDist[i][j].a ) * Polynomial::S( normI - i) + 
//                 ( constDist[i][j].c - constDist[i][j].a ) * Polynomial::S( normJ - j) + 
//                 ( constDist[i][j].a - constDist[i][j].b - constDist[i][j].c + constDist[i][j].d) * Polynomial::S( normI - i) * Polynomial::S(normJ - j) / 1.3f;

//             x+= gridLength;
//             y+= gridLength;
//         }
//     }
    
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "polynomial.h"

Noise::Noise(int height, int width, int sqGridLen):
terrainHeight(height),
terrainWidth(width),
sqGridLen(sqGridLen),
Xindex(0),
Zindex(0)
{
    gridVert temp;
    temp.a = 0;
    temp.b = 0;
    temp.c = 0;
    temp.d = 0;

    //initialize the grid vertices
    for( int i = 0; i < terrainHeight / sqGridLen; i++ ){

        vert.push_back(std::vector<gridVert>());

        for( int j = 0; j < terrainWidth / sqGridLen; j++ ){

            vert[i].push_back(temp);
        }

    }

    genGridPointsHeight();
   
}

void Noise::genGridPointsHeight(){

    //the the area between grid point height will be drawn by the polynomial function
    //to have uniformity the grid point height of near grids should be equal
    
    //let us assume the max height we want is 200

    //vertexData
    float u, v;

    for( int i = 0; i < terrainHeight / sqGridLen; i++ ){

        for( int j = 0; j < terrainWidth / sqGridLen; j++){

            //this is the main portion of algorithm
            //this will determine the height changes in the terrain
            // u = 3.68 * (float)rand() / (float)RAND_MAX;
            // v = 3.68 * (float)rand() / (float)RAND_MAX;
            u = 3.68 * sin(i);
            v = 3.68 * sin(j);

            //there is some problem in here
            if( i == 0 || j == 0 ){

                vert[i][j].a = 2 * u * v * ( u + v ) - 1;
                vert[i][j].b = vert[i][j].a;
                vert[i][j].c = vert[i][j].a;
                vert[i][j].d = vert[i][j].a;

            }else{

                vert[i][j].a = 2 * u * v * ( u + v ) - 1;
                vert[i-1][j].b = vert[i][j].a;
                vert[i][j-1].c = vert[i][j].a;
                vert[i-1][j-1].d = vert[i][j].a;
            }
            
        }
    }
}

float Noise::poly(float in){

    //helps to smooth the cuve around the grid vertex
    return (3 * in * in - 2 * in * in * in);
}

float Noise::polynomial(int xCoord, int zCoord){

    //find out the constant and value of i
    int i = xCoord / sqGridLen;
    int j = zCoord / sqGridLen;

    //the value of grid vertex
    int iCoord = (xCoord % sqGridLen == 0) ? xCoord : sqGridLen + sqGridLen * ((int)xCoord / sqGridLen);
    int jCoord = (zCoord % sqGridLen == 0) ? zCoord : sqGridLen + sqGridLen * ((int)zCoord / sqGridLen);

    
   //this portion is for smooth interpolation between different heights in the terrain
   float result =
   vert[i][j].a +
   (vert[i][j].b - vert[i][j].a)*poly( xCoord - iCoord ) + 
   (vert[i][j].c - vert[i][j].a)*poly(  zCoord - jCoord ) +
   (vert[i][j].a - vert[i][j].b - vert[i][j].c + vert[i][j].d)*poly( xCoord - iCoord )*poly( zCoord - jCoord );

    return result;
}


