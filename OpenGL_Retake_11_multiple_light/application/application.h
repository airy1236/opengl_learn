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

	bool init(const int &w = 800, const int &h = 600);

	bool update();

	bool destory();

	GLFWwindow* getWindow();

	void setmousecallback(mousecallback callback) { mouse = callback; }
	void setscrollcallback(mousecallback callback) { scroll = callback; }

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