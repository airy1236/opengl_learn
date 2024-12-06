#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define App Application::getInstance()


class Application {
public:

	~Application();

	static Application* getInstance();

	GLFWwindow* getWindow();

	bool init(const int& w = 800, const int& h = 600);

	bool update();

	bool destory();



private:

	Application();



	static Application* Instance;

	GLuint width{ 0 };
	GLuint height{ 0 };



	GLFWwindow* window = nullptr;

};