#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/application.h"
#include "glframework/camera/camera.h"
#include "glframework/shader/shader.h"
#include "glframework/texture/texture.h"

int src_width = 1920;
int src_height = 1080;

GLuint vbo, object_vao, light_vao;
Camera* camera = nullptr;
Shader *object_shader = nullptr, *light_shader = nullptr;
Texture* object_texture = nullptr;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

float lastX = src_width / 2.0f;
float lastY = src_height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void LoadCamera() {
	camera = new Camera(glm::vec3(0.0, 0.0, 3.0));
}

void processInput() {
	
	if (glfwGetKey(app->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(app->getWindow(), true);

	if (glfwGetKey(app->getWindow(), GLFW_KEY_W) == GLFW_PRESS) camera->KeyMove("FORWARD", deltaTime);
	if (glfwGetKey(app->getWindow(), GLFW_KEY_A) == GLFW_PRESS) camera->KeyMove("LEFT", deltaTime);
	if (glfwGetKey(app->getWindow(), GLFW_KEY_S) == GLFW_PRESS) camera->KeyMove("BACKWARD", deltaTime);
	if (glfwGetKey(app->getWindow(), GLFW_KEY_D) == GLFW_PRESS) camera->KeyMove("RIGHT", deltaTime);
}

void MouseCallback(double xposIn, double yposIn) {
	float xpos = xposIn;
	float ypos = yposIn;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera->MouseMove(xoffset, yoffset);
}

void ScrollCallback(double xoffset, double yoffset) {
	camera->MouseScroll(yoffset);
}

void LoadVertex() {
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &object_vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(object_vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glGenVertexArrays(1, &light_vao);
	glBindVertexArray(light_vao);

	glBindBuffer(1, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void LoadShader() {
	object_shader = new Shader("assets/shaders/object_shader/vertex.vert", "assets/shaders/object_shader/fragment.frag");
	light_shader = new Shader("assets/shaders/light_shader/vertex.vert", "assets/shaders/light_shader/fragment.frag");
}

void LoadTexture() {
	object_texture = new Texture("assets/textures/5.png", 0);
}

void render() {
	
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	processInput();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*lightColor.x = sin(glfwGetTime() * 2.0f);
	lightColor.y = sin(glfwGetTime() * 0.7f);
	lightColor.z = sin(glfwGetTime() * 1.3f);*/
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
	
	
	object_shader->begin();
	object_shader->setVec3("viewPos", camera->getPosition());
	//light
	object_shader->setVec3("light.position", lightPos);
	object_shader->setVec3("light.ambient", ambientColor);
	object_shader->setVec3("light.diffuse", diffuseColor);
	object_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//merterial
	object_shader->setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
	object_shader->setVec3("material.diffuse", 0.0f, 0.51f, 0.51f);
	object_shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	object_shader->setFloat("material.shininess", 25.0f);
	//m, v, p
	glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)src_width / (float)src_height, 0.1f, 100.0f);
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	object_shader->setMat4("projection", projection);
	object_shader->setMat4("view", view);
	object_shader->setMat4("model", model);
	glBindVertexArray(object_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	object_shader->end();
	

	light_shader->begin();
	light_shader->setMat4("projection", projection);
	light_shader->setMat4("view", view);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2));
	light_shader->setMat4("model", model);
	glBindVertexArray(light_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	light_shader->end();
}


int main() {
	app->init(src_width, src_height);
	app->setMouseCallback(MouseCallback);
	app->setScrollCallback(ScrollCallback);

	glViewport(0, 0, src_width, src_height);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glEnable(GL_DEPTH_TEST);

	LoadCamera();
	LoadShader();
	LoadVertex();

	while (app->update()) {
		render();
	}
	
	app->destory();

	delete camera;
	delete object_shader;
	delete light_shader;
	delete object_texture;

	return 0;
}