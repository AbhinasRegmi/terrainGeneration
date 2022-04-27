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

//create textures for shadow maps
Texture::Texture(unsigned int mapWidth,  unsigned int mapHeight):
mapWidth(mapWidth),
mapHeight(mapHeight)
{

    //update the texture numbers
    thisTexture = totalTextures;
    totalTextures++;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mapWidth, mapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

}

//attach textures for sky box
Texture::Texture(std::vector<std::string> faces){

    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    

    unsigned char* data;

    for(unsigned int i = 0; i < faces.size(); i++){

        data = stbi_load(faces[i].c_str(), &width, &height, &nChannels, 0);

        if( data ){
            //determine the channel type
            GLenum colorMode = GL_RGB;
            switch (nChannels) {
            case 1:
                colorMode = GL_RED;
            break;
            case 4:
                colorMode = GL_RGBA;
            break;
            };

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        }else{

            std::cout<<"cube maps couldn't be found or they are not in order as cubemap1, cubemap2 ,etc"<<"\n";
        }

        stbi_image_free(data);

    }  

    //set the wrapping parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


void Texture::setTexture(Shader& textureShader, const std::string &name){
    
    textureShader.useShader();
    textureShader.setInt(name.c_str(), thisTexture);
};

void Texture::setSkybox(Shader& textureShader, const std::string &name){
    textureShader.useShader();
    textureShader.setInt(name.c_str(), 0);
}


void Texture::useTexture(){

    glActiveTexture(GL_TEXTURE0 + thisTexture);
    glBindTexture(GL_TEXTURE_2D, ID);
};

void Texture::useSkybox(){

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}