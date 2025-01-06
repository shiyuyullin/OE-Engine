#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "stb_image.h"
#include "glad/glad.h"

using namespace std;

class Cubemap
{
public:
	unsigned int texture;
	Cubemap(vector<string> faces);
	~Cubemap();
};


Cubemap::Cubemap(vector<string> faces)
{
	stbi_set_flip_vertically_on_load(false);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int width, height, nrChannels;
	for(unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces.at(i).c_str(), &width, &height, &nrChannels, 0);
		if (data) 
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			cout << "cubemap failed to load for path: " << faces.at(i) << endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	cout << "Cubemap loaded" << endl;
}

Cubemap::~Cubemap()
{
	glDeleteTextures(1, &this->texture);
	if (!glIsTexture(this->texture))
	{
		cout << "Texture " << this->texture << " is deleted." << endl;
	}
	else
	{
		cout << "Texture " << this->texture << " is still valid." << endl;
	}
}