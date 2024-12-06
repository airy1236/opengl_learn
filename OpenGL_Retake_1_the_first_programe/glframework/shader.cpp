#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	//声明读取shader代码的变量
	std::string vertexCode, fragmentCode;
	//声明读取shder文件的变量
	std::ifstream vShaderFile, fShaderFile;

	//保证ifstream遇到问题的时候可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//将shader文件里的代码拷贝到字符串流里
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//关闭文件
		vShaderFile.close();
		fShaderFile.close();

		//将shader代码从字符串流中读取出来，转化成字符串形式
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR: Shader File Error\n" << e.what() << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	//创建Shader程序(VertexShader, FragmentShader)
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	//为Shader程序注入源码
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	//对VertexShader程序执行编译
	glCompileShader(vertex);
	//检查编译结果
	CheckShaderError(vertex, "COMPILE");

	//对FragmentShader程序执行编译
	glCompileShader(fragment);
	//检查编译结果
	CheckShaderError(fragment, "COMPILE");

	//创建一个Program可执行程序
	mProgram = glCreateProgram();

	//将编译好的VertexShader和FragmentShader程序放入Program
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);

	//将VertexShader和FragmentShader链接进Program，形成最终可执行Shader程序
	glLinkProgram(mProgram);
	//链接错误检查
	CheckShaderError(mProgram, "LINK");

	//清理
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
	//获取uniform变量的位置
	GLuint location = glGetUniformLocation(mProgram, name.c_str());
	//设置变量的值
	glUniform1f(location, value);
}