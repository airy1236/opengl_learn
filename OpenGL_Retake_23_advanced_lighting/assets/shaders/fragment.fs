#version 460 core

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main() {
	vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
	vec3 ambient = 0.05 * color;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(0.0, dot(lightDir, normal));
	vec3 diffuse = diff * color;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0;
	if(blinn) {
		vec3 halfway = normalize(lightDir + viewDir);
		spec = pow(max(0.0, dot(normal, halfway)), 32.0);
	}
	else {
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}
	vec3 specular = vec3(0.3) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}