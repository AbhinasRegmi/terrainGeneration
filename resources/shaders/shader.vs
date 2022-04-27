#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 FragPos;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;


void main(){

    //necessary data for shadow calculations
    vs_out.FragPos = vec3(model * vec4(aPosition, 1.0f));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

    FragPos = vec3( model * vec4( aPosition, 1.0f));
    normal = mat3(transpose(inverse(model))) * normalize(aNormal);

    gl_Position = projection * view * model * vec4( aPosition, 1.0f);
}