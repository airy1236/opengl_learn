#version 460 core

out vec4 FragColor;

uniform vec3 cubecolor;
uniform vec3 lightcolor;

void main() {
	FragColor = vec4(lightcolor * cubecolor, 1.0);
}