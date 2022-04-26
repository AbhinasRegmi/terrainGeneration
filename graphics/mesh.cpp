#include "mesh.h"


//define member functions and constructors
Mesh::Mesh(int height, int width, int resolution_in):Fractal(height, width){

    //Vertex temp;

    //we will generate the vertices for our terrain

       for( int i = 0; i < height; i++ ){

            for( int j = 0; j < width; j++ ){

                //position         
                vertices.push_back(i);
                vertices.push_back(genHeight(i, j));
                vertices.push_back(j);

                //normals we will generate normals later
            }
    }


    //we will generate the indices for our terrain
    for( unsigned int i = 0; i < height - 1; i += resolution){

        for( unsigned int j = 0; j < width; j += resolution){

            for (unsigned int k = 0; k < 2; k++ ){

                indices.push_back(j + width * ( i + k * resolution));
            }
        }
    }

    setup();
};

void Mesh::setup(){

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    //send the data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    //how to interpret data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
};

void Mesh::draw(Shader& shader){

    const int totalNumStrips = (height - 1) / resolution;
    const int verticesPerStripe = (width / resolution) * 2;

    shader.useShader();
    //draw the finally
    glBindVertexArray(VAO);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //we will draw strip by strip
    for( unsigned int i = 0; i < totalNumStrips; i++  ){

            glDrawElements(
            GL_TRIANGLE_STRIP,
            verticesPerStripe + 2,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * (verticesPerStripe + 2) * i));
    }  

    glBindVertexArray(0);
};

void Mesh::clean(){

    glDeleteVertexArrays(1, &VAO);
};

