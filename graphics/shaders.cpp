#include "shaders.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath){

    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    //enable error checking for the fileStreams
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        //open the shader files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //close the flie buffer
        vShaderFile.close();
        fShaderFile.close();

        //convert the stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch(std::ifstream::failure e)
    {
        std::cout<<"Error in reading Files"<<std::endl;
    }
    
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    //create and compile vertex and fragment shaders
    unsigned int vertexId, fragmentId;
    int success;
    char infoLog[512];

    vertexId = glCreateShader(GL_VERTEX_SHADER);
    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

    //link the vertex source and compile
    glShaderSource(vertexId, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexId);

    //get if any errors
    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
        std::cout << "vertex Shader couldn't be compiled.." << std::endl << infoLog << std::endl;
    };

    //link the fragment source and compile
    glShaderSource(fragmentId, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentId);

    //get if any errors
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
        std::cout << "fragment Shader couldn't be compiled.." << std::endl << infoLog << std::endl;
    };

    //create a shader program with the above vertex and fragment shader
    ID = glCreateProgram();
    glAttachShader(ID, vertexId);
    glAttachShader(ID, fragmentId);
    glLinkProgram(ID);

    //get if any errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "the vertex and fragment shader couldn't be linked" << std::endl << infoLog << std::endl;
    };

    //delete the liked shaders
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
};

void Shader::useShader(){

    glUseProgram(ID);
};

void Shader::deleteShader(){

    glDeleteProgram(ID);
};

void Shader::setInt(const std::string &name, int value) const{

    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
};   

void Shader::setFloat(const std::string &name, float value) const{

    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);

};

void Shader::setMat4(const std::string &name, glm::mat4 value) const{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
};

void Shader::setVec3f(const std::string &name, glm::vec3 value) const{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMaterial(const std::string &name) const{
    //the materials.txt should have this setting
    //material  ambient diffuse speculat shininess
    std::string materialFilePath = "/home/abhinas/devs/C++/OpenGl/Lighting/resources/materials.txt";
    std::ifstream materialFilePointer;
    std::vector<float> materialValues;
    bool isMaterialName = true;
    std::string terminator = " ";
    std::string singleMaterial;

    //open the material file
    materialFilePointer.open(materialFilePath);

    if( !materialFilePointer ){

        std::cout<<"Couldn't find materials.txt";
        materialFilePointer.close();
        return;
    }
    
    while( !materialFilePointer.eof() ){

        getline(materialFilePointer, singleMaterial);
        if( (singleMaterial.find(name)  == 0) && (name.length() != 0) ){

            size_t pos = 0;

            while( (pos = singleMaterial.find(terminator) ) != std::string::npos ){

                if( isMaterialName ){
                    singleMaterial.erase(0, pos + terminator.length());
                    isMaterialName = false;
                }

                materialValues.push_back(stof(singleMaterial.substr(0, pos)));
                singleMaterial.erase(0, pos + terminator.length());

            }

            break;
        }
    }

    materialFilePointer.close();

    if(isMaterialName){
        std::cout<<"Couldn't find the material "<<name<<" in material.txt"<<std::endl;
    }

    Material material;

    material.ambient.x = materialValues[0];
    material.ambient.y = materialValues[1];
    material.ambient.z = materialValues[2];

    material.diffuse.x = materialValues[3];
    material.diffuse.y = materialValues[4];
    material.diffuse.z = materialValues[5];

    material.specular.x = materialValues[6];
    material.specular.y = materialValues[7];
    material.specular.z = materialValues[8];

    material.shininess = materialValues[9];

    setVec3f("material.ambient", material.ambient);
    setVec3f("material.diffuse", material.diffuse);
    setVec3f("material.specular", material.specular);
    setFloat("material.shininess", material.shininess);
}


