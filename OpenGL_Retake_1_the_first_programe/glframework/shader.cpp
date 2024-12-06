#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	//������ȡshader����ı���
	std::string vertexCode, fragmentCode;
	//������ȡshder�ļ��ı���
	std::ifstream vShaderFile, fShaderFile;

	//��֤ifstream���������ʱ������׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//��shader�ļ���Ĵ��뿽�����ַ�������
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//�ر��ļ�
		vShaderFile.close();
		fShaderFile.close();

		//��shader������ַ������ж�ȡ������ת�����ַ�����ʽ
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR: Shader File Error\n" << e.what() << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	//����Shader����(VertexShader, FragmentShader)
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//ΪShader����ע��Դ��
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	//��VertexShader����ִ�б���
	glCompileShader(vertex);
	//��������
	CheckShaderError(vertex, "COMPILE");

	//��FragmentShader����ִ�б���
	glCompileShader(fragment);
	//��������
	CheckShaderError(fragment, "COMPILE");

	//����һ��Program��ִ�г���
	mProgram = glCreateProgram();

	//������õ�VertexShader��FragmentShader�������Program
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);

	//��VertexShader��FragmentShader���ӽ�Program���γ����տ�ִ��Shader����
	glLinkProgram(mProgram);
	//���Ӵ�����
	CheckShaderError(mProgram, "LINK");

	//����
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	delete this;
}

void Shader::begin() {
	glUseProgram(mProgram);
}

void Shader::end() {
	glUseProgram(0);
}

void Shader::CheckShaderError(GLuint target, std::string type) {
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
		glGetProgramiv(mProgram, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, NULL, infolog);
			std::cout << "ERROR:Sahder Link Error\n" << infolog << std::endl;
		}
	}
	else {
		std::cout << "ERROR:The Type Is Wrong" << std::endl;
	}
}

void Shader::setFloat(const std::string& name, const float value) {
	//��ȡuniform������λ��
	GLuint location = glGetUniformLocation(mProgram, name.c_str());
	//���ñ�����ֵ
	glUniform1f(location, value);
}