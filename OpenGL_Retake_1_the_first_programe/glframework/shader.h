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

	void begin();//��ʼʹ�õ�ǰshader

	void end();//����ʹ�õ�ǰshader

	void setFloat(const std::string& name, const float value);

private:
	//���shader��program�Ĵ���
	void CheckShaderError(GLuint target, std::string type);
public:
	GLuint mProgram{ 0 };


};