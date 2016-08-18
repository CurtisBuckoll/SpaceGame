
/*
File:			Shader.h
Description:	A class for loading/compiling shaders and creating a shader program object.
Author:			Curtis Buckoll/learnOpenGL.com
*/

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // Include glew to get required OpenGL headers

class Shader
{
private:
	// Program ID
	GLuint _program;

public:
	// Constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// Destructor
	~Shader();
	//Getter - return Program ID
	GLuint GetProgramID();
	// Use the program
	void Use();
};

