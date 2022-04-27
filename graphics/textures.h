#ifndef TEXTURES_H
#define TEXTURES_H

#include <string>
#include "shaders.h"

class Texture{

    public:
        static int totalTextures;
        unsigned int ID;
        unsigned int mapWidth, mapHeight;

        Texture(std::string texturePath);
        Texture(std::vector<std::string>);
        Texture(unsigned int mapWidth,  unsigned int mapHeight);

        void setTexture(Shader& textureShader, const std::string &name);
        void setSkybox(Shader& textureShader, const std::string &name);

        void useTexture();
        void useSkybox();

    private:
        int thisTexture;

};

#endif
