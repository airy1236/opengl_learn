#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using mousecallback = void(*)(double, double);

#define app Application::getInstance()

class Application {
public:
	
	~Application();

	static Application* getInstance();

	bool init(const int& w, const int& h);

	bool update();

	bool destory();

	GLFWwindow* Getwindow();

	void setMouseCallback(mousecallback callback) { mouseCallback = callback; }
	void setScrollCallback(mousecallback callback) { scrollCallback = callback; }

private:

	static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
	
	static void ScrollCallback(GLFWwindow* windwo, double xoffset, double yoffset);

private:

	Application();
	
	static Application* Instance;

	uint32_t width{ 0 };
	uint32_t height{ 0 };

	GLFWwindow* window{ nullptr };

	mousecallback mouseCallback{ nullptr };
	mousecallback scrollCallback{ nullptr };
};
