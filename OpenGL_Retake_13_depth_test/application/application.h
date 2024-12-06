#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define App Application::getInstance()

using mousecallback = void(*)(double, double);

class Application {
public:

	~Application();

	static Application* getInstance();

	GLFWwindow* getWindow();

	bool init(const int& w = 800, const int &h = 600);

	bool update();

	bool destory();

	void setMouseCallback(mousecallback callback) { mouse = callback; }
	void setScrollCallback(mousecallback callback) { scroll = callback; }

private:

	Application();

	static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static Application* Instance;

	GLuint width{ 0 };
	GLuint height{ 0 };

	mousecallback mouse{ nullptr };
	mousecallback scroll{ nullptr };

	GLFWwindow* window = nullptr;

};