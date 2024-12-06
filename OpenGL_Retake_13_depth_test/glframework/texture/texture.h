#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class mTexture {
public:

	mTexture(const char* path, unsigned int u);

	~mTexture();

	void begin();

private:

	GLuint texture{ 0 };

	int width{ 0 };
	int height{ 0 };

	unsigned int unit{ 0 };

};