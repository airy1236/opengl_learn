#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application/application.h"
#include "glframework/shader.h"
#include "glframework/texture.h"

//GLM library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint vao;
Shader* shader = nullptr;
Texture* texture1 = nullptr, *texture2 = nullptr;

void LoadData() {
	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLuint vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void LoadShader() {
	shader = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
}

void LoadTexture() {
	texture1 = new Texture("assets/textures/little_tomboy.jpg", 0);
	//texture2 = new Texture("assets/textures/little_tomboy.jpg", 1);
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	shader->begin();

	shader->setInt("texture1", 0);
	//shader->setInt("texture2", 1);

	//transform
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(1.5f, 2.0f, 2.0f));
	shader->setTransform(transform, "transform");
	
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	shader->end();
}



int main() {
	app->init(800, 600);
	
	glViewport(0, 0, 800, 600);

	glClearColor(0.4f, 0.3f, 0.5f, 1.0f);

	LoadData();
	LoadShader();
	LoadTexture();

	while (app->update()) {

		
		render();

	}

	app->destory();
	delete shader;
	delete texture1;
	delete texture2;

	return 0;
}