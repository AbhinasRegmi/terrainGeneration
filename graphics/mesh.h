#ifndef MESH_H
#define MESH_H

#include <vector>

#include "noise/fractal.h"
#include "shaders.h"
#include "glm/glm.hpp"


class Mesh : public Fractal{

    public:
        unsigned int VAO;
       
        std::vector<float> vertices;
        std::vector<unsigned int>indices;

        Mesh(int height, int width, int resolution_in);

        void draw(Shader& shader);

        void clean();

    private:
        int resolution;
        int height;
        int width;
        unsigned int VBO, EBO;

        void setup();


};

#endif