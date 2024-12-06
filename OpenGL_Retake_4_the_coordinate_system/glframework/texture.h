#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture {
public:

	Texture(const std::string &path, unsigned int u);

	~Texture();

	void bind();

private:

	GLuint texture;

	int width{ 0 };
	int height{ 0 };

	unsigned int unit{ 0 };

};