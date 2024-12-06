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

	Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path = nullptr);

	~Shader();

	GLuint getPrograme();

	void begin();

	void end();

	void setBool(const std::string& nmae, bool value);

	void setInt(const std::string& name, int value);

	void setFloat(const std::string& name, float value);

	void setVec2(const std::string& name, glm::vec2 value);
	void setVec2(const std::string& name, float x, float y);

	void setVec3(const std::string& name, glm::vec3 value);
	void setVec3(const std::string& name, float x, float y, float z);

	void setVec4(const std::string& name, glm::vec4 value);
	void setVec4(const std::string& name, float x, float y, float z, float w);

	void setMat2(const std::string& name, glm::mat2 vlaue);

	void setMat3(const std::string& name, glm::mat3 value);

	void setMat4(const std::string& name, glm::mat4 value);

private:

	void CheckShaderError(int target, std::string mode);

	GLuint program;

};

class ComputeShader
{
public:

    ComputeShader(const char* computePath);
    
    void begin();

    void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setVec2(const std::string& name, const glm::vec2& value) const;

    void setVec2(const std::string& name, float x, float y) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;

    void setVec3(const std::string& name, float x, float y, float z) const;

    void setVec4(const std::string& name, const glm::vec4& value) const;

    void setVec4(const std::string& name, float x, float y, float z, float w);

    void setMat2(const std::string& name, const glm::mat2& mat) const;

    void setMat3(const std::string& name, const glm::mat3& mat) const;

    void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
    GLuint programe;

    void CheckShaderError(int target, const std::string mode);
};