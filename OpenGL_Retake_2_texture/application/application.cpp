#include "application.h"

Application* Application::instance = nullptr;

Application::Application() {}

Application* Application::getInstance() {
	if (instance == nullptr) instance = new Application();
	return instance;
}

bool Application::init(const int& w, const int& h) {
	width = w;
	height = h;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "OpenGL_Retake_2_pipline", NULL, NULL);
	if (window == nullptr) return false;

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return false;
	}
	return true;
}

bool Application::update() {
	if (glfwWindowShouldClose(window)) return false;

	glfwPollEvents();

	glfwSwapBuffers(window);

	return true;
}

bool Application::destory() {
	glfwTerminate();
	return true;
}

Application::~Application() {}