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

	Shader(const char* vertex_path, const char* fragment_path);
	~Shader();

	void begin();

	void end();

	void setInt(const char* target, int value);

	void setFloat();

	void setTransform(glm::mat4 matrix, const char* variable);

private:

	void CheckShaderError(GLuint target, std::string mode);

	GLuint program{ 0 };

};