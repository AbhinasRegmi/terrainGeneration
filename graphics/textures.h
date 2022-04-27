#ifndef TEXTURES_H
#define TEXTURES_H

#include "shaders.h"

class Texture{

    public:
        static int totalTextures;
        unsigned int ID;
        unsigned int mapWidth, mapHeight;

        Texture(std::string texturePath);
        Texture(unsigned int mapWidth,  unsigned int mapHeight);

        void setTexture(Shader& textureShader, const std::string &name);
        void useTexture();

    private:
        int thisTexture;

};

#endif
