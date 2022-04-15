#include "shaders.h"

#include "keyboard.h"
#include "mouse.h"
#include "camera.h"
#include "screen.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(double dt);

//camera positions
glm::vec3 cameraPos = glm::vec3(0.0f, 20.0f, 0.0f);
Camera ourCamera(cameraPos);

//make our screen class
Screen ourScreen;

//unifrom values
glm::mat4 model = glm::mat4(1);
glm::mat4 view = glm::mat4(1);
glm::mat4 projection = glm::mat4(1);

//lighting
glm::vec3 lightPos(1.0f, 1.0f, 1.0f);

//for dt calcualtion
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(){

    ourScreen.init("Terrain", 800, 800);

    ourScreen.callBacks();
    ourScreen.loadOpenGl();

    //load and compile the shaders
    Shader ourShader(
    "/home/abhinas/devs/C++/terrainGeneration/resources/shaders/shader.vs",
    "/home/abhinas/devs/C++/terrainGeneration/resources/shaders/shader.fs"
    );

    //the dimension and no of channels in image
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    //load the height map
    unsigned char* data = stbi_load(
    "/home/abhinas/devs/C++/terrainGeneration/resources/maps/everest_heightmap.png",
    &width,
    &height,
    &nrChannels,
    0
    );

    //limit our scaling
    // float yScale = 64.0f / 256.0f, yShift = 16.0f;

    //resolution for output triangles
    //lower the better
    int resolution = 3;

    //generate our height map
    std::vector<float> vertices;

    //convert int to unsigned int
    unsigned int pixelSize = nrChannels;

    for( int i = 0; i < height; i++ ){

        for( int j = 0; j < width; j++ ){

            //only take first channel value as our height
            unsigned char* pixelOffset = data + (j + width * i) * pixelSize;
            unsigned char y = pixelOffset[0];   //height i.e y coord

            //push the vertex values
            //make x range from -height/2 to +height/2
            vertices.push_back( - height / 2.0f + i);
            //normalize the height and scale to desired range
            vertices.push_back( y - 100.0f);
            //make z range from -widht/2 to +width/2
            vertices.push_back( - width / 2.0f + j); //vertices.z
        }
    }

    //free the data
    stbi_image_free(data);

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
    const int verticesPerStripe = (width / resolution) * 2;  //this determines the width of rendered seen

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    //bind ebo to element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //draw in wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        projection = glm::perspective(glm::radians(ourCamera.zoom), (float)ourScreen.screenWidth / (float)ourScreen.screenHeight, 0.1f, 700.0f);

        //send the tranformations
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        //render the cube
        glBindVertexArray(VAO);

        //draw the terrain strip by strip 
        //don't try to draw everything at once
        for( unsigned int i = 0; i < totalNumStrips; i++  ){

            glDrawElements(
            GL_TRIANGLE_STRIP,
            verticesPerStripe + 2,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * (verticesPerStripe + 2) * i));
        }

        ourScreen.newFrame();
    }

   //free spaces
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    ourScreen.terminate();
    

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
