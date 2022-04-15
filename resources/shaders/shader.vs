#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    
    color = aPosition;

    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}