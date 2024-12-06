#version 460 core

#define N 4

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct DirLight { //定向光
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	float diff = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	
	float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	return (ambient + diffuse + specular);
}

struct PointLight { //点光源
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constanst;
	float linear;
	float quadratic;
};
uniform PointLight pointLights[N];

vec3 ColPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	float diff = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constanst + light.linear * dist + light.quadratic * dist * dist);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutoff;
	float outercutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform SpotLight spotLight;

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	float diff = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outercutoff;
	float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}

void main() {
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;
	result += CalDirLight(dirLight, normal, viewDir);
	for(int i = 0; i < N; i++) {
		result += ColPointLight(pointLights[i], normal, FragPos, viewDir);
	}
	result += CalSpotLight(spotLight, normal, FragPos, viewDir);

	FragColor = vec4(result, 1.0f);

}