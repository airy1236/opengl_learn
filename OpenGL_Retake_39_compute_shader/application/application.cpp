#include "application.h"

Application* Application::Instance = nullptr;

Application::Application() {}

Application::~Application() {}

Application* Application::getInstance() {
	if (Instance == nullptr) Instance = new Application();
	return Instance;
}

GLFWwindow* Application::getWindow() {
	return window;
}

bool Application::init(const int& w, const int& h) {
	width = w;
	height = h;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(width, height, "OpenGL_Retake_32_SSAO", NULL, NULL);
	if (window == nullptr) return false;

	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, this);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLad error" << std::endl;
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

