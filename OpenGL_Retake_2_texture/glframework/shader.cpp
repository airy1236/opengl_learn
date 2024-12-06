#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexcode, fragmentcode;
	std::ifstream vertexshaderfile, fragmentshaderfile;

	vertexshaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentshaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexshaderfile.open(vertexPath);
		fragmentshaderfile.open(fragmentPath);

		std::stringstream vertexshaderstream, fragmentshaderstream;
		vertexshaderstream << vertexshaderfile.rdbuf();
		fragmentshaderstream << fragmentshaderfile.rdbuf();

		vertexshaderfile.close();
		fragmentshaderfile.close();

		vertexcode = vertexshaderstream.str();
		fragmentcode = fragmentshaderstream.str();
	}
	catch (std::ifstream::failure &e) {
		std::cout << "Shader file error\n" << e.what() << std::endl;
	}

	const char* vertexshadersource = vertexcode.c_str();
	const char* fragmentshadersource = fragmentcode.c_str();

	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertexshadersource, NULL);
	glShaderSource(fragment, 1, &fragmentshadersource, NULL);

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

Shader::~Shader() {}

void Shader::begin() {
	glUseProgram(program);
}

void Shader::end() {
	glUseProgram(0);
}

void Shader::CheckShaderError(const GLuint target, std::string type) {
	int success;
	char infolog[1024];
	if (type == "COMPILE") {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(target, 1024, NULL, infolog);
			std::cout << "ERROR:Sahder Compile Error\n" << infolog << std::endl;
		}
	}
	else if (type == "LINK") {
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infolog);
			std::cout << "ERROR:Sahder Link Error\n" << infolog << std::endl;
		}
	}
	else {
		std::cout << "ERROR:The Type Is Wrong" << std::endl;
	}
}