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
	vec3 position;  //�۹��λ��
    vec3 direction; //�۹ⷽ��
	float cutoff;   //���й��
	float cuteroff; //���й��

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;  //�������1.0
	float linear;    //һ�������˥������ǿ��
	float quadratic; //��������εݼ�����ǿ��
};
uniform Light light;

uniform vec3 viewPos;

void main() {
	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	//diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); //���߷���
	float diff = max(0.0, dot(Normal, lightDir));
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
	
	//soft_edge
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.cuteroff;
	float intensity = clamp((theta - light.cuteroff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	//attenuation
	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}