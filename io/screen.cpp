#include "screen.h"

#include "keyboard.h"
#include "mouse.h"

unsigned int Screen::screenWidth = 800;
unsigned int Screen::screenHeight = 800;

void Screen::frameBufferSizeCallBack(GLFWwindow* window, int width, int height){
   
    screenWidth = width;
    screenHeight = height;

    glViewport(0, 0, width, height);
}

Screen::Screen(){
    window = nullptr;
}

void Screen::init(std::string name, int width, int height){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);


    if( !window ){
        std::cout<<"Failed to create a Window";
        
    }

    glfwMakeContextCurrent(window);
};

void Screen::callBacks(){

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallBack);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallBack);
};

void Screen::loadOpenGl(){

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){

        std::cout << "opengl couldn't be loaded by glad" << std::endl;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
};

void Screen::update(){

    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor(0.0f, 0.7098f, 0.8862f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void Screen::newFrame(){

    glfwSwapBuffers(window);
    glfwPollEvents();
};

bool Screen::shouldClose(){

    return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool close){

    glfwSetWindowShouldClose(window, close);
};

void Screen::terminate(){

    glfwTerminate();
};

void Screen::processInput(double dx){

};
