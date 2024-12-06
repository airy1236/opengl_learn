#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 WorldPos;

uniform mat3 normalMatrix;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    TexCoords = aTexCoords;
    Normal = normalMatrix * aNormal;
    WorldPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}