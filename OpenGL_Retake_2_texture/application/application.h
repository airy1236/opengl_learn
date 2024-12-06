#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define app Application::getInstance()

class Application {
public:
	~Application();

	static Application* getInstance();

	bool init(const int &w = 800, const int &h = 600);
	bool update();
	bool destory();

private:

	Application();
	static Application* instance;
	
	uint32_t width{ 0 };
	uint32_t height{ 0 };

	GLFWwindow* window{ nullptr };

};