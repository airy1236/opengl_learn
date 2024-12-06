#include "Shader.h"

Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path) {
	std::string vertex_code, fragment_code;
	std::ifstream vertex_shader_file, fragment_shader_file;

	vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertex_shader_file.open(vertex_shader_path);
		fragment_shader_file.open(fragment_shader_path);

		std::stringstream vertex_shader_code_stream, fragment_shader_code_stream;
		vertex_shader_code_stream << vertex_shader_file.rdbuf();
		fragment_shader_code_stream << fragment_shader_file.rdbuf();

		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_code = vertex_shader_code_stream.str();
		fragment_code = fragment_shader_code_stream.str();
	}
	catch (std::ifstream::failure &e) {
		std::cout << "Shader file error\n" << e.what() << std::endl;
	}

	const char* vertex_shader_source = vertex_code.c_str();
	const char* fragment_shader_source = fragment_code.c_str();

	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertex_shader_source, NULL);
	glShaderSource(fragment, 1, &fragment_shader_source, NULL);

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
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infolog);
			std::cout << "Program link error\n" << infolog << std::endl;
		}
	}
}

void Shader::setTransform(glm::mat4 trans, const char* variable) {
	unsigned int transform_location = glGetUniformLocation(program, variable);
	glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans));
}

void Shader::setInt(const char* target, int value) {
	glUniform1i(glGetUniformLocation(program, target), value);
}