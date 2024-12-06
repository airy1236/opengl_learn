#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_iamge/stb_image.h"

Texture::Texture(const char* path, unsigned int unit) {
	int channels;
	stbi_set_flip_vertically_on_load_thread(true);
	unsigned char* data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0 + unit);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, channels, 0, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

Texture::~Texture() {
	if (texture != 0) glDeleteTextures(1, &texture);
}