#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>

struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class Shader
{
public:
    //id of our shader program
    unsigned int ID;
    
    Shader(const char* vertexPath, const char* fragmentPath);

    void useShader();
    void deleteShader();

    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
    void setVec3f(const std::string &name, glm::vec3 value) const;
};

#endif
