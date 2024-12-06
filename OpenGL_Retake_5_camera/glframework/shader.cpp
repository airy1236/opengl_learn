#include "shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path) {
	std::string vertex_code, fragment_code;
	std::ifstream vertex_shader_file, fragment_shader_file;

	vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertex_shader_file.open(vertex_path);
		fragment_shader_file.open(fragment_path);

		std::stringstream vertex_stream, fragment_stream;
		vertex_stream << vertex_shader_file.rdbuf();
		fragment_stream << fragment_shader_file.rdbuf();

		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "Shader file error\n" << e.what() <<std::endl;
	}

	const char* vertex_code_source = vertex_code.c_str();
	const char* fragment_code_source = fragment_code.c_str();

	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertex_code_source, NULL);
	glShaderSource(fragment, 1, &fragment_code_source, NULL);

	glCompileShader(vertex);
	CheckShaderError(vertex, "COMPILE");
	glCompileShader(fragment);
	CheckShaderError(fragment, "COMPILE");

	program = glCreateProgram();

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);
	CheckShaderError(program, "LINK");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {};

void Shader::begin() {
	glUseProgram(program);
}

void Shader::end() {
	glUseProgram(0);
}

void Shader::CheckShaderError(GLuint target, std::string mode) {
	int success;
	char infolog[1024];
	if (mode == "COMPILE") {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(target, 1024, NULL, infolog);
			std::cout << "Shader compile error\n" << infolog << std::endl;
		}
	}
	else if (mode == "LINK") {
		glGetProgramiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infolog);
			std::cout << "Program link error\n" << infolog << std::endl;
		}
	}
}

void Shader::setInt(const char* target, const int var) {
	glUniform1i(glGetUniformLocation(program, target), var);
}

void Shader::setMat4(const char* target, glm::mat4 matrix4) {
	glUniformMatrix4fv(glGetUniformLocation(program, target), 1, GL_FALSE, &matrix4[0][0]);
}