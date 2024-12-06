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

	void begin();//开始使用当前shader

	void end();//结束使用当前shader

	void setFloat(const std::string& name, const float value);

private:
	//检查shader和program的错误
	void CheckShaderError(GLuint target, std::string type);
public:
	GLuint mProgram{ 0 };


};