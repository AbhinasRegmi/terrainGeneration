#ifndef TEXTURES_H
#define TEXTURES_H

#include "shaders.h"

class Texture{

    public:
        static int totalTextures;
        unsigned int ID;

        Texture(std::string texturePath);

        void setTexture(Shader& textureShader, const std::string &name);
        void useTexture();

    private:
        int thisTexture;

};

#endif
