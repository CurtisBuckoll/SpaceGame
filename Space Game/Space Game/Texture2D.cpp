#include "Texture2D.h"
#include <SOIL/SOIL.h>
#include <iostream>

Texture2D::Texture2D()
{
	_textureID = 0;
	_width = 0;
	_height = 0;
	std::string _filepath = "";
}

Texture2D::Texture2D(std::string filepath, TEX_TYPE image_type)
{
	_filepath = filepath;

	// Set texture ID
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	// Defailt Paramaters. Will add shit to modify these after
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Texture loading
	unsigned char* image = SOIL_load_image((const char*)&filepath[0], &_width, &_height, 0, SOIL_LOAD_AUTO);
	if (image == nullptr) {
		std::cout << "Invalid image file. Image loading failed." << std::endl;
	}

	// Bind image data with GL_TEXTURE_2D, generate mimpmaps
	if (image_type == PNG_RGBA) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, &image[0]);
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind/free resources
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}


Texture2D::~Texture2D()
{

}

GLuint Texture2D::GetTextureID()
{
	return _textureID;
}

std::string Texture2D::GetFilepath()
{
	return _filepath;
}

void Texture2D::Delete()
{
	glDeleteTextures(1, &_textureID);
}