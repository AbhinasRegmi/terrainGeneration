// #version 330 core

// layout (location = 0) in vec3 aPosition;
// layout (location = 1) in vec3 aNormal;

// out vec3 color;
// out vec3 normal;
// out vec3 FragPos;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// void main(){
    
//     color = aPosition;
//     FragPos = vec3( model * vec4(aPosition, 1.0f));
//     normal = mat3(transpose(inverse(model))) * aNormal;

//     gl_Position = projection * view * model * vec4(aPosition, 1.0f);
// }

//simple vertex shader
#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){

    gl_Position = projection * view * model * vec4( aPosition, 1.0f);
}