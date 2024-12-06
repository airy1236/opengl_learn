#version 460 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct Light {
	vec3 Position;
	vec3 Color;
};

uniform Light lights[16];
uniform sampler2D diffuseMap;

void main() {
	vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	//ambient
	vec3 ambient = 0.1 * color;
	
	vec3 lighting = vec3(0.0);
	for(int i = 0; i < 16; ++i) {
		//diffuse
		vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
		float diff = max(0.0, dot(lightDir, normal));
		vec3 diffuse = lights[i].Color * diff * color;
		float dist = length(fs_in.FragPos - lights[i].Position);
		diffuse *= 1.0 / (dist * dist);
		lighting += diffuse;
	}

	FragColor = vec4(ambient + lighting, 1.0);
}