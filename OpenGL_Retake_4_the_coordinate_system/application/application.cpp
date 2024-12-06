#include "application.h"

Application* Application::Instance = nullptr;

Application::Application() {};

Application::~Application() {};

Application* Application::getInstance() {
	if (Instance == nullptr) Instance = new Application();
	return Instance;
}

bool Application::init(const int &w, const int &h) {
	width = w;
	height = h;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "OpenGL_Retake_4_the_coordinate_system", NULL, NULL);
	if (window == nullptr) return false;

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Glad error\n");
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