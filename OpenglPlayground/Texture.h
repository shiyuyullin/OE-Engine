#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <glad/glad.h>
#include <string>
#include "stb_image.h"

using namespace std;

class Texture
{
public:
	unsigned int texture;
	Texture(string path, GLint internalFormat, GLenum format);
	~Texture();
};


Texture::Texture(string path, GLint internalFormat, GLenum format)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height, nrChannels;
	unsigned char* image = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "failed to load wood image" << endl;
	}
	// free image memory
	stbi_image_free(image);
}

Texture::~Texture()
{

}



#endif // !TEXTURE_H


