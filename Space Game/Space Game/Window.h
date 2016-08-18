/* 
File:			Window.h
Description:	A class for creating a managing the paramaters of a single GLFW window object.
Author:			Curtis Buckoll
*/

#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow* _window;
	GLuint _width;
	GLuint _height;

public:
	Window(GLuint width, GLuint height, const char* title);
	~Window();

	void DefineViewport();
	void Close();
	void SwapBuffers();

	// Getters
	bool ShouldClose();
	GLFWwindow* getWindowPtr();
	int getWidth();
	int getHeight();

};

