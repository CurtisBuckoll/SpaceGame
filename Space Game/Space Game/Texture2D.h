#pragma once

#include <string>
#include <GL/glew.h>

enum TEX_TYPE {
	PNG_RGBA = 0,
	PNG_RGB
};

class Texture2D
{
private:
	GLuint _textureID;
	int _width;
	int _height;
	std::string _filepath;  ///< Use filepath for sorting textures, maybe

public:
	Texture2D(); ///< ONLY use default constructor for initializing objects in other classes
	Texture2D(std::string filepath, TEX_TYPE image_type);
	~Texture2D();

	GLuint GetTextureID();
	std::string GetFilepath();

	// Manual Destructor
	void Delete();
};

