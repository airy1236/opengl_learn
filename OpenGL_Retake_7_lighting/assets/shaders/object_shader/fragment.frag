#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 cubeColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	//环境光
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	//漫反射
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//镜面反射
	float specularStrength = 0.9;
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * cubeColor;
	FragColor = vec4(result, 1.0);
}