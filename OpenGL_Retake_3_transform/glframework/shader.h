#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

//GLM library
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
public:

	Shader(const char* vertex_shader_path, const char* fragment_shader_path);
	~Shader();

	void begin();

	void end();

	void setTransform(glm::mat4 trans, const char* variable);

	void setInt(const char* target, int value);

private:

	void CheckShaderError(GLuint target, std::string mode);

	GLuint program{ 0 };

};