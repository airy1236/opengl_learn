#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define app Application::getInstance()

using mouseCallback = void(*)(double x, double y);

class Application {
public:
	
	~Application();

	static Application* getInstance();

	bool init(const int& w = 800, const int& h = 600);

	bool update();

	bool destory();

	GLFWwindow* getWindow() { return window; }

	void setMouseCallback(mouseCallback callback) { mouse = callback; }
	void setScrollCallback(mouseCallback callback) { scroll = callback; }

private:
	
	static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    
	Application();

	static Application* Instance;

	GLuint width{ 0 };
	GLuint height{ 0 };

	GLFWwindow* window{ nullptr };
	
	mouseCallback mouse{ nullptr };
	mouseCallback scroll{ nullptr };
};