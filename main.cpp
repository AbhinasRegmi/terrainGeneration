#include "shaders.h"
#include "textures.h"
#include "noise/simplex.h"
#include "noise/fractal.h"

#include "keyboard.h"
#include "mouse.h"
#include "camera.h"
#include "screen.h"

#include <vector>
#include <string>

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
glm::vec3 lightDirection(1.0f, 0.5f, 0.1f);
glm::vec3 lightFront(0.0f);
glm::vec3 lightUp(0.0f, 1.0f, 0.0f);

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

    Shader ourShadowShader(
    "/home/abhinas/devs/C++/terrainGeneration/resources/shaders/framebuffer.vs",
    "/home/abhinas/devs/C++/terrainGeneration/resources/shaders/framebuffer.fs"
    );

    Shader ourSkyShader(
    "/home/abhinas/devs/C++/terrainGeneration/resources/shaders/sky.vs",
    "/home/abhinas/devs/C++/terrainGeneration/resources/shaders/sky.fs");

   
    Texture ourSoil("/home/abhinas/devs/C++/terrainGeneration/resources/maps/soil.jpg");
    Texture ourGrass("/home/abhinas/devs/C++/terrainGeneration/resources/maps/grass.jpg");

   
    Texture ourShadow(4500, 4500);

    //the dimension of our final terrain
    int width, height, squareGridLength;

    //choose squareGridLength such that both width and height is divisible i.e 100/5 = 20
    width = 1000;
    height = 1000;
    squareGridLength = 10;

    //resolution for output triangles
    int resolution = 9;

    //generate our plane
    std::vector<float> vertices;

    //create object of our noise
    Fractal ourNoise(height, width, 12, 3.0, 0.867);


    for( int i = 0; i < height; i++ ){

        for( int j = 0; j < width; j++ ){

            //position         
            vertices.push_back(i);
            vertices.push_back( ourNoise.genHeight(i, j) );
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
    const int verticesPerStripe = (width / resolution) * 2;

    //send data to gpu
    unsigned int VAO, VBO, EBO, FBO;

    //generate the array buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenFramebuffers(1, &FBO);

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

    //bind fbo to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ourShadow.ID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


   
    //draw in wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //run the main loop
    while( !ourScreen.shouldClose() ){

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(deltaTime);

        //first render the scene to framebuffer
        glViewport(0, 0, ourShadow.mapWidth, ourShadow.mapHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //do something
        //set the scene from view of light source
        glm::mat4 lightProjection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 1.0f, 600.0f);
        glm::mat4 lightView = glm::lookAt(
            lightDirection,
            lightDirection + lightFront,
            lightUp
        );

        model = glm::mat4(1.0f);
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        ourShadowShader.useShader();
        ourShadowShader.setMat4("model", model);
        ourShadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        //finallly render in framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        for( unsigned int i = 0; i < totalNumStrips; i++  ){

            glDrawElements(
            GL_TRIANGLE_STRIP,
            verticesPerStripe + 2,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * (verticesPerStripe + 2) * i));
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //render the scene to screen
        ourScreen.update();

      
        //use our Shader
        ourShader.useShader();

        //set the transformations
        view = ourCamera.getViewMatrix();
        projection = glm::perspective(glm::radians(ourCamera.zoom), (float)ourScreen.screenWidth / (float)ourScreen.screenHeight, 0.1f, 600.0f);
        model = glm::scale(model, glm::vec3(0.8));
        

        //send the tranformations
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        //send the camera position
        ourShader.setVec3f("viewPos", ourCamera.cameraPos);
        ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        //set light properties
        ourShader.setVec3f("light.direction", lightDirection);
        ourShader.setVec3f("light.ambient", glm::vec3(0.5));
        ourShader.setVec3f("light.diffuse", glm::vec3(1.0f));
        ourShader.setVec3f("light.specular", glm::vec3(0.14f));

        //send textues
        //--------------------------------------------------------//

        ourSoil.useTexture();   //tex0
        ourGrass.useTexture();  //tex1

        ourShader.setInt("depthMap", 2);    
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ourShadow.ID); //tex2

        //-------------------------------------------------//

        ourSoil.setTexture(ourShader, "material.diffuse[0]");
        ourGrass.setTexture(ourShader, "material.diffuse[1]");
        ourShader.setVec3f("material.ambient[0]", glm::vec3(0.345f));
        ourShader.setVec3f("material.ambient[1]", glm::vec3(0.012f));
        // ourShader.setVec3f("material.diffuse[0]", glm::vec3(0.545, 0.27, 0.074));
        // ourShader.setVec3f("material.diffuse[1]", glm::vec3(0.337, 0.49, 0.30));
        ourShader.setVec3f("material.specular", glm::vec3(0.35));
        ourShader.setFloat("material.shininess", 2.0f);

        //render the cube
        glBindVertexArray(VAO);

       // draw the terrain strip by strip 
        for( unsigned int i = 0; i < totalNumStrips; i++  ){

            glDrawElements(
            GL_TRIANGLE_STRIP,
            verticesPerStripe + 2,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * (verticesPerStripe + 2) * i));
        }


        ourScreen.newFrame();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &FBO);
    ourScreen.terminate();
    

    return 0;
}


void processInput(double dt){
    
    if( Keyboard::key(GLFW_KEY_ESCAPE) ){
        ourScreen.setShouldClose(true);
    }

    if( Keyboard::keyWentDown(GLFW_KEY_L)){

        //update light position as camera
        lightDirection = ourCamera.cameraPos;
        lightFront = ourCamera.cameraFront;
        lightUp = ourCamera.cameraUp;
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
