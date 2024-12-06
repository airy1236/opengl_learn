#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define App Application::getInstance()

using mousecallback = void(*)(double, double);

class Application {
public:
	
	~Application();

	static Application* getInstance();

	bool init(const int& width = 800, const int& height = 600);

	bool update();

	bool destory();

	GLFWwindow* getWindow();

	void setMouseCallback(mousecallback callback) { mouse = callback; }
	void setScrollCallback(mousecallback callback) { scroll = callback; }

private:

	static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	Application();

	static Application* Instance;

	GLuint width{ 0 };
	GLuint height{ 0 };
	
	GLFWwindow* window{ nullptr };

	mousecallback mouse{ nullptr };
	mousecallback scroll{ nullptr };

};