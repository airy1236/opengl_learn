#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void begin();
	
	void end();

	void setFloat(const std::string name, const float value);

	void setInt(const std::string name, const int value);

private:
	void CheckShaderError(GLuint target, std::string type);

	GLuint program{ 0 };
};