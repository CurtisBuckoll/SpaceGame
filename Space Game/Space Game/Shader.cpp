#include "Shader.h"


// Constructor reads and builds the shader
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	/* ---- 1. Load Shaders from file ---- */

	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::string vertexCode = "";
	std::string fragmentCode = "";
	std::string line = "";
	
	// Open files
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	if (vertexFile.fail()) {
		std::cout << "Failed to open vertex shader." << std::endl;
		return;
	}
	if (fragmentFile.fail()) {
		std::cout << "Failed to open fragment shader." << std::endl;
		return;
	}

	// Read from files	
	while (!vertexFile.eof()) {
		std::getline(vertexFile, line);
		vertexCode = vertexCode + line + '\n';
	}
	while (!fragmentFile.eof()) {
		std::getline(fragmentFile, line);
		fragmentCode = fragmentCode + line + '\n';
	}

	vertexFile.close();
	fragmentFile.close();

	/* ---- 2. Build and compile shader program ---- */

	const GLchar* vertexCodeString = vertexCode.c_str();
	const GLchar* fragmentCodeString = fragmentCode.c_str();
	GLint success;
	GLchar infoLog[512];

	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCodeString, NULL);
	glCompileShader(vertexShader);

	// Check for Vertex Shader compile time errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCodeString, NULL);
	glCompileShader(fragmentShader);

	// Check for Fragment Shader compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link shaders
	_program = glCreateProgram();
	glAttachShader(_program, vertexShader);
	glAttachShader(_program, fragmentShader);
	glLinkProgram(_program);

	// Check for linking errors
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	
	// Free Memory (no longer need shaders once program is created)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{

}

GLuint Shader::GetProgramID()
{
	return _program;
}

// Use the program
void Shader::Use()
{
	glUseProgram(_program);
}
