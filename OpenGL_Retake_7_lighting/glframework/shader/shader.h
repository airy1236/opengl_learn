#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

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

	void setBool(const std::string &name, const bool value);

	void setInt(const std::string& name, const int value);

	void setFloat(const std::string& name, const float value);

	void setVec2(const std::string& name, const glm::vec2& value);
	void setVec2(const std::string& name, const float x, float y);

	void setVec3(const std::string& name, const glm::vec3& vlaue);
	void setVec3(const std::string& name, float x, float y, float z);

	void setVec4(const std::string& name, const glm::vec4& value);
	void setVec4(const std::string& name, float x, float y, float z, float w);

	void setMat2(const std::string& name, const glm::mat2& value);

	void setMat3(const std::string& name, const glm::mat3& value);

	void setMat4(const std::string& name, const glm::mat4& value);

private:

	void CheckShaderError(GLuint target, std::string mode);

	GLuint program{ 0 };

};