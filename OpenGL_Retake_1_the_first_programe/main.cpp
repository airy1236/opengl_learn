#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>

#include "glframework/shader.h"

Shader* shader = nullptr;
GLuint vao;

//ËæÊ±¼ä¸³ÓèÑÕÉ«
void color_transform() {
	SYSTEMTIME time;
	GetLocalTime(&time);
	double currentTime = time.wHour + time.wMinute + time.wSecond;
	const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f,
							  (float)cos(currentTime) * 0.5f + 0.5f,
															   0.0f,
															   1.0f };
	glClearColor(color[1], color[2], color[3], color[4]);
}

void loadshader() {
	shader = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
}

void loadposition() {
	float position[] = { 0.25f, -0.25f, 0.5f,
						-0.25f, -0.25f, 0.5f,
						0.25f, 0.25f, 0.5f,
		                -0.25f, 0.25f, 0.5f
	};
	float color[] = {1.0f, 0.0f, 0.0f,
		             0.0f, 1.0f, 0.0f,
		             0.0f, 0.0f, 1.0f,
		             0.4f, 0.0f, 0.3f
	};
	unsigned int index[] = {0, 1, 2, 2, 1, 3};

	//VBO
	GLuint position_vbo = 0, color_vbo = 0;
	glGenBuffers(1, &position_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	//EBO
	GLuint ebo = 0;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	//VAO

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Bind vao & ebo
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	shader->begin();

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "retake", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glViewport(0, 0, 800, 600);

	loadshader();
	loadposition();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

	    color_transform();

		render();



		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}