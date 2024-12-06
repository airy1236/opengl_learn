#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrMap;
uniform bool hdr;
uniform float exposure;

void main() {
	const float gamma = 2.2;
	vec3 hdrColor = texture(hdrMap, TexCoords).rgb;
	if(hdr) {
		vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
		mapped = pow(mapped, vec3(1.0 / gamma));
		FragColor = vec4(mapped, 1.0);
	}
	else {
		vec3 mapped = pow(hdrColor, vec3(1.0 / gamma));
		FragColor = vec4(mapped, 1.0);
	}
}