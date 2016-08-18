#include "Window.h"


// Parameterized Constructor
Window::Window(GLuint width, GLuint height, const char* title)
{
	// Define Window Settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window object
	_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	_width = width;
	_height = height;

	// Set window to current working window
	glfwMakeContextCurrent(_window);
}

// Destructor
Window::~Window()
{
	glfwDestroyWindow(_window);
}

// Set window viewing dimensions for OpenGL
void Window::DefineViewport()
{
	int WIDTH, HEIGHT;
	glfwGetFramebufferSize(_window, &WIDTH, &HEIGHT);
	glViewport(0, 0, WIDTH, HEIGHT);
}

void Window::Close()
{
	glfwSetWindowShouldClose(_window, GL_TRUE);
}

bool Window::ShouldClose()
{
	if (glfwWindowShouldClose(_window)) {
		return true;
	}
	else {
		return false;
	}
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(_window);
}

// Getters

GLFWwindow* Window::getWindowPtr()
{
	return _window;
}

int Window::getWidth()
{
	return _width;
}

int Window::getHeight()
{
	return _height;
}
