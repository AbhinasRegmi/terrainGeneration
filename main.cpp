#include "shaders.h"
#include "noise/simplex.h"
#include "noise/fractal.h"
//#include "mesh.h"

#include "keyboard.h"
#include "mouse.h"
#include "camera.h"
#include "screen.h"

#include <fstream>

void processInput(double dt);

//camera positions
glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 0.0f);
Camera ourCamera(cameraPos);

//make our screen class
Screen ourScreen;

//unifrom values
glm::mat4 model = glm::mat4(1);
glm::mat4 view = glm::mat4(1);
glm::mat4 projection = glm::mat4(1);

//lighting
glm::vec3 lightPos(500.0f, 500.0f, 100.0f);

//for dt calcualtion
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(){

    ourScreen.init("Terrain Polynomial", 800, 800);

    ourScreen.callBacks();
    ourScreen.loadOpenGl();

    //load and compile the shaders
    Shader ourShader(
    "/home/abhinas/devs/C++/terrainGeneration/resources/shaders/shader.vs",
    "/home/abhinas/devs/C++/terrainGeneration/resources/shaders/shader.fs"
    );


    //Mesh ourTerrain(100, 100, 2);

    //the dimension of our final terrain
    int width, height, squareGridLength;

    //choose squareGridLength such that both width and height is divisible i.e 100/5 = 20
    width = 1000;
    height = 1000;
    squareGridLength = 5;

    //resolution for output triangles
    //lower the better
    int resolution = 3;

    //generate our plane
    std::vector<float> vertices;

    //create object of our noise
    Fractal ourNoise(height, width, 18, 2.3, 0.9);


    for( int i = 0; i < height; i++ ){


        for( int j = 0; j < width; j++ ){

            //position         
            vertices.push_back(i);
            vertices.push_back( ourNoise.genHeight(i, j));
            vertices.push_back(j);

            //dummy normals
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    //compute the normals
    for( int i = 0; i < height; i++ ){

        for( int j = 0; j < width; j++ ){

            int currVertex = (i * width + j) * 6;
            int sideVertex = currVertex + 6;
            int downVertex = currVertex + width * 6;

            glm::vec3 e1 = glm::vec3(vertices[currVertex] - vertices[sideVertex], vertices[currVertex + 1] - vertices[sideVertex + 1], vertices[currVertex + 2] - vertices[sideVertex + 2]);
            glm::vec3 e2 = glm::vec3(vertices[downVertex] - vertices[sideVertex], vertices[downVertex + 1] - vertices[sideVertex + 1], vertices[downVertex + 2] - vertices[sideVertex + 2]);

            glm::vec3 norm = glm::cross(e2, e1);

            vertices[currVertex + 3] += norm.x;
            vertices[currVertex + 4] += norm.y;
            vertices[currVertex + 5] += norm.z;

            vertices[sideVertex + 3] += norm.x;
            vertices[sideVertex + 4] += norm.y;
            vertices[sideVertex + 5] += norm.z;

            vertices[downVertex + 3] += norm.x;
            vertices[downVertex + 4] += norm.y;
            vertices[downVertex + 5] += norm.z;
        }
    }


    //create indices to draw the traingles from a square
    std::vector<unsigned int> indices;

    for( unsigned int i = 0; i < height - 1; i += resolution){

        for( unsigned int j = 0; j < width; j += resolution){

            for (unsigned int k = 0; k < 2; k++ ){

                indices.push_back(j + width * ( i + k * resolution));
            }
        }
    }

    //to draw terrain strip by strip find total no_of strip
    const int totalNumStrips = (height - 1) / resolution;
    const int verticesPerStripe = (width / resolution) * 2;  //this determines the width_height_map of rendered seen

    //send data to gpu
    unsigned int VAO, VBO, EBO;

    //generate the array buffer, buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind the vao with vbo
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    //enable the attrib pointer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));

    //bind ebo to element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //draw in wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //run the main loop
    while( !ourScreen.shouldClose()){

        ourScreen.update();

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(deltaTime);

        //use our Shader
        ourShader.useShader();

        //set the transformations
        model = glm::mat4(1.0f);
        view = ourCamera.getViewMatrix();
        projection = glm::perspective(glm::radians(ourCamera.zoom), (float)ourScreen.screenWidth / (float)ourScreen.screenHeight, 0.1f, 250.0f);

        //send the tranformations
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        //-------------------------------------------------------------------------------//

        //send the camera position
        ourShader.setVec3f("viewPos", ourCamera.cameraPos);

        //set light properties
        ourShader.setVec3f("light.position", lightPos);
        ourShader.setVec3f("light.ambient", glm::vec3(0.1f));
        ourShader.setVec3f("light.diffuse", glm::vec3(1.0f));
        ourShader.setVec3f("light.specular", glm::vec3(0.1f));

        //rgb(53, 18, 2)222, 184, 135
        //set material properties
        ourShader.setVec3f("material.ambient", glm::vec3(0.2f));
        ourShader.setVec3f("material.diffuse", glm::vec3((float)222/255, (float)184/255, (float)135/255));
        ourShader.setVec3f("material.specular", glm::vec3(0.1f));
        ourShader.setFloat("material.shininess", 1.0f);

        //render the cube
        glBindVertexArray(VAO);

       // draw the terrain strip by strip 
       // don't try to draw everything at once
        for( unsigned int i = 0; i < totalNumStrips; i++  ){

            glDrawElements(
            GL_TRIANGLE_STRIP,
            verticesPerStripe + 2,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * (verticesPerStripe + 2) * i));
        }

        // ourTerrain.draw(ourShader);

        ourScreen.newFrame();
    }

   //free spaces
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    ourScreen.terminate();
    //ourTerrain.clean();
    

    return 0;
}


void processInput(double dt){
    
    if( Keyboard::key(GLFW_KEY_ESCAPE) ){
        ourScreen.setShouldClose(true);
    }

    //move camera with keys
    if( Keyboard::key(GLFW_KEY_W)){
        ourCamera.updateCameraPos(cameraMovement::FORWARD, dt);
    }

    if( Keyboard::key(GLFW_KEY_S)){
        ourCamera.updateCameraPos(cameraMovement::BACKWARD, dt);
    }
    
    if( Keyboard::key(GLFW_KEY_A)){
        ourCamera.updateCameraPos(cameraMovement::LEFT, dt);
    }
    
    if( Keyboard::key(GLFW_KEY_D)){
        ourCamera.updateCameraPos(cameraMovement::RIGHT, dt);
    }

    if( Keyboard::key(GLFW_KEY_SPACE)){
        ourCamera.updateCameraPos(cameraMovement::UP, dt);
    }

    if( Keyboard::key(GLFW_KEY_LEFT_CONTROL)){
        ourCamera.updateCameraPos(cameraMovement::DOWN, dt);
    }

    //move camera with mouse
    double dx = Mouse::getDX(), dy = Mouse::getDY();

    if( dx != 0 || dy != 0 ){
        ourCamera.updateCameraDirection(dx, dy);
    }

    //adjust zoom
    double scrolldy = Mouse::getScrollDY();
    
    if( scrolldy != 0 ){
    
        ourCamera.updateCameraZoom(scrolldy);
    }
};
