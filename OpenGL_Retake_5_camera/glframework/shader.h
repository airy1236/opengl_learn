#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:

	Shader(const char* vertex_path, const char* fargment_path);
	~Shader();

	void begin();

	void end();

	void setInt(const char* target, int var);

	void setMat4(const char* target, glm::mat4 matrix);

private:
	
	void CheckShaderError(GLuint target, std::string mode);

	GLuint program{ 0 };

};