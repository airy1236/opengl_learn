#include <iostream>
#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/application.h"
#include "glframework/shader/shader.h"
#include "glframework/camera/camera.h"
#include "glframework/model/model.h"

#ifdef NV
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
#endif

int scr_width = 800;
int scr_height = 600;


float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0, 0.0, 3.0));

void processInput() {
	if (glfwGetKey(Application::getInstance()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(Application::getInstance()->getWindow(), true);
	}

	if (glfwGetKey(Application::getInstance()->getWindow(), GLFW_KEY_W) == GLFW_PRESS) camera.KeyMove("FORWARD", deltaTime);
	if (glfwGetKey(Application::getInstance()->getWindow(), GLFW_KEY_S) == GLFW_PRESS) camera.KeyMove("BACKWARD", deltaTime);
	if (glfwGetKey(Application::getInstance()->getWindow(), GLFW_KEY_A) == GLFW_PRESS) camera.KeyMove("LEFT", deltaTime);
	if (glfwGetKey(Application::getInstance()->getWindow(), GLFW_KEY_D) == GLFW_PRESS) camera.KeyMove("RIGHT", deltaTime);

}

void MouseMove(double xPos, double yPos) {
	float xpos = xPos;
	float ypos = yPos;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.MouseMove(xoffset, yoffset);
}

void MouseScroll(double xoffset, double yoffset) {
	camera.MouseScroll(yoffset);
}

int main() {
	Application::getInstance()->init(scr_width, scr_height);
	Application::getInstance()->setMouseCallback(MouseMove);
	Application::getInstance()->setScrollCallback(MouseScroll);

	glViewport(0, 0, scr_width, scr_height);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glEnable(GL_DEPTH_TEST);
	
	
	Shader shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
	Model model("assets/models/bunny.obj");

	while (Application::getInstance()->update()) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.begin();
		//v, p
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		//model
		glm::mat4 m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(0.0));
		m_model = glm::scale(m_model, glm::vec3(1.0));
		shader.setMat4("model", m_model);
		model.Draw(shader);
		shader.end();
	}

	Application::getInstance()->destory();



	return 0;
}