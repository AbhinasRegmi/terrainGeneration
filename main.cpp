#include "shaders.h"

#include "keyboard.h"
#include "mouse.h"
#include "camera.h"
#include "screen.h"

void processInput(double dt);

//camera positions
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
Camera ourCamera(cameraPos);

//make our screen class
Screen ourScreen;

//unifrom values
glm::mat4 model = glm::mat4(1);
glm::mat4 modelLamp = glm::mat4(1);
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


    //run the main loop
    while( !ourScreen.shouldClose()){

        ourScreen.update();

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(deltaTime);


        ourScreen.newFrame();
    }

   //free spaces
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
