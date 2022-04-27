#ifndef TEXTURES_H
#define TEXTURES_H

#include <string>
#include "shaders.h"

class Texture{

    public:
        static int totalTextures;
        unsigned int ID;

        Texture(std::string texturePath);
        Texture::Texture(unsigned int mapWidth,  unsigned int mapHeight);

        void setTexture(Shader& textureShader, const std::string &name);

        void useTexture();

        unsigned int mapWidth;
        unsigned int mapHeight;
    private:
        int thisTexture;

};

#endif
