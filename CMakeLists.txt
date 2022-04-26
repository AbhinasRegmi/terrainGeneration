cmake_minimum_required(VERSION 3.1)

#set the project name
project("terrain")

#specify the c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#specify includes for external include and libs
include_directories(external/include)
link_directories(external/libs)


#specify the includes for graphics and io
include_directories(graphics)
include_directories(io)

#build library files
add_library(io_keyboard io/keyboard.cpp)
add_library(io_mouse io/mouse.cpp)
add_library(io_camera io/camera.cpp)
add_library(io_screen io/screen.cpp)
add_library(shaders graphics/shaders.cpp)
add_library(simplex graphics/noise/simplex.cpp)
add_library(fractal graphics/noise/fractal.cpp)

#build the target
add_executable(${PROJECT_NAME} main.cpp glad.c graphics/noise/simplex.cpp)


#link the required libraries 
target_link_libraries(${PROJECT_NAME}
GL dl glfw
shaders fractal
io_keyboard
io_mouse
io_camera
io_screen
)

#create a zip file (app) for distribution
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
include(InstallRequiredSystemLibraries)
include(CPack)

