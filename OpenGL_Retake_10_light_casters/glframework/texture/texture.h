#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture {
public:

	Texture(const char* path, unsigned int u);

	~Texture();

	void begin();

private:

	GLuint texture{ 0 };

	int width{ 0 };
	int height{ 0 };

	unsigned int unit{ 0 };

};