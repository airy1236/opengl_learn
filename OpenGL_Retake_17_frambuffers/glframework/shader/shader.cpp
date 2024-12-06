#include "shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path) {
	std::string vertex_code, fragment_code, geometry_code;
	std::ifstream vertex_file, fragment_file, geometry_file;

	vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometry_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);


	try {
		vertex_file.open(vertex_path);
		fragment_file.open(fragment_path);

		std::stringstream vertex_stream, fragment_stream;
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		vertex_file.close();
		fragment_file.close();

		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();

		if (geometry_path != nullptr) {
			geometry_file.open(geometry_path);
			std::stringstream geometry_stream;
			geometry_stream << geometry_file.rdbuf();
			geometry_file.close();
			geometry_code = geometry_stream.str();
		}
	}
	catch (std::ifstream::failure& e) {
		std::cout << "Shader file error\n" << e.what() << std::endl;
	}

	const char* vertex_code_source = vertex_code.c_str();
	const char* fragment_code_source = fragment_code.c_str();

	GLuint vertex, fragment, geometry;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertex_code_source, NULL);
	glShaderSource(fragment, 1, &fragment_code_source, NULL);

	glCompileShader(vertex);
	CheckShaderError(vertex, "COMPILE");
	glCompileShader(fragment);
	CheckShaderError(fragment, "COMPILE");

	if (geometry_path != nullptr) {
		const char* geometry_code_source = geometry_code.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometry_code_source, NULL);
		glCompileShader(geometry);
		CheckShaderError(geometry, "COMPILE");
	}

	program = glCreateProgram();

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	if (geometry_path != nullptr) {
		glAttachShader(program, geometry);
	}

	glLinkProgram(program);
	CheckShaderError(program, "LINK");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometry_path != nullptr) {
		glDeleteShader(geometry);
	}
}

Shader::~Shader() {}

void Shader::begin() {
	glUseProgram(program);
}

void Shader::end() {
	glUseProgram(0);
}

GLuint Shader::getProgram() {
	return program;
}

void Shader::CheckShaderError(int target, const std::string mode) {
	int success;
	char infolog[1024];
	if (mode == "COMPILE") {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(target, 1024, NULL, infolog);
			std::cout << "Shader compile error\n" << infolog << std::endl;
		}
	}
	if (mode == "LINK") {
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infolog);
			std::cout << "Program link error\n" << infolog << std::endl;
		}
	}
}

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) {
	glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) {
	glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, glm::vec4 value) {
	glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, glm::mat2 value) {
	glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string& name, glm::mat3 value) {
	glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}