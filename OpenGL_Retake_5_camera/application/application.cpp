#include "application.h"

Application* Application::Instance = nullptr;

Application::Application() {}

Application::~Application() {}

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

	window = glfwCreateWindow(width, height, "OpenGL_Retake_5_camera", NULL, NULL);
	if (window == nullptr) return false;

	glfwMakeContextCurrent(window);

	//◊¢≤·
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	//◊¢≤· Û±Í ‰»Î
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(window, this);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Glad error" << std::endl;
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

GLFWwindow* Application::Getwindow() {
	return window;
}

void Application::MouseCallback(GLFWwindow* window, double xPos, double yPos) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->MouseCallback != nullptr) self->mouseCallback(xPos, yPos);
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->ScrollCallback != nullptr) self->scrollCallback(xoffset, yoffset);
}