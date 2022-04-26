#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../external/include/stb_image.h"

int Texture::totalTextures = 0;

Texture::Texture(std::string texturePath){

    //set this texture no.
    thisTexture = totalTextures;
    //increment totalTextures
    totalTextures++;
    //set the name of the texture

    int width, height, nChannels;

    //load the texture image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nChannels, 0);

    //find no of channels in the texture
    GLenum colorMode = GL_RGB;
    switch (nChannels) {
    case 1:
        colorMode = GL_RED;
        break;
    case 4:
        colorMode = GL_RGBA;
        break;
    };

    //create a texture
    glGenTextures(1, &ID);

    if (data) {
        //bind the texture
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        //set the texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cout << "texture not found at the given path" << std::endl;
    }

    stbi_image_free(data);

};

void Texture::setTexture(Shader& textureShader, const std::string &name){
    
    textureShader.useShader();
    textureShader.setInt(name.c_str(), thisTexture);
};


void Texture::useTexture(){

    glActiveTexture(GL_TEXTURE0 + thisTexture);
    glBindTexture(GL_TEXTURE_2D, ID);
};