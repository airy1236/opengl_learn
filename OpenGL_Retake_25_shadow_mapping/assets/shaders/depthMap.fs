#version 460 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D depthMap;
uniform float far;
uniform float near;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main() {
	float depthValue = texture(depthMap, TexCoords).r;
	FragColor = vec4(vec3(LinearizeDepth(depthValue) / far), 1.0); // perspective
	//FragColor = vec4(vec3(depthValue), 1.0); //ortho
}