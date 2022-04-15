#ifndef SCREEN_H
#define SCREEN_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

class Screen{

    public:
        static unsigned int screenWidth;
        static unsigned int screenHeight;

        static void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);

        Screen();

        void init(std::string name, int width , int height );
        void callBacks();
        void loadOpenGl();

        void update();
        void processInput(double dx);
        void newFrame();

        bool shouldClose();
        void setShouldClose(bool close);

        void terminate();

        GLFWwindow* window;
};

#endif