#version 460 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct Light {
	vec3 position;
    //vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant; //常项，总是1.0
	float linear;    //一次项，线性衰减光照强度
	float quadratic; //二次项，二次递减光照强度
};
uniform Light light;

uniform vec3 viewPos;

void main() {
	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	//diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}