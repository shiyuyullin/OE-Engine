#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	// active the shader
	void use();

	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
	void setVec3f(const string& name, float v1, float v2, float v3) const;
	void setVec3fv(const string& name, int count, glm::vec3 vec3);
	void setVec4f(const string& name, float v1, float v2, float v3, float v4) const;
	void setMat3f(const string& name, int count, bool transpose, glm::mat3 transform);
	void setMat4f(const string& name, int count, bool transpose, glm::mat4 transform);
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// variables for storing source code of shaders
	string vertexShaderCode;
	string fragmentShaderCode;
	// files
	ifstream vertexShaderFile;
	ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		stringstream vertexShaderStream, fragmentShaderStream;
		// read file's buffer contents into stream
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		// close file
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexShaderCode = vertexShaderStream.str();
		fragmentShaderCode = fragmentShaderStream.str();
	}
	catch (ifstream::failure& e)
	{
		cout << "Shader file read unsuccessful" << endl;
	}
	const char* vShaderCode = vertexShaderCode.c_str();
	const char* fShaderCode = fragmentShaderCode.c_str();

	unsigned int vertexShader, fragmentShader;
	int success;
	// create and compile vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	// check if vertex shader is compiled successfully
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		cout << "vertex shader compilcation failed" << endl;
	}
	// create and compile fragment shader
	fragmentShader  = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	// check if fragment shader compiled successfully 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		cout << "fragment shader compilation failed" << endl;
	}
	// create shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// checkpoint for successful linking of the shader program
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		cout << "failed to link shader program" << endl;	
	}
	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3f(const string& name, float v1, float v2, float v3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

inline void Shader::setVec3fv(const string& name, int count, glm::vec3 vec3)
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, glm::value_ptr(vec3));
}

void Shader::setVec4f(const string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

inline void Shader::setMat3f(const string& name, int count, bool transpose, glm::mat3 transform)
{
	if (transpose)
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), count, GL_TRUE, glm::value_ptr(transform));

	}
	else
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), count, GL_FALSE, glm::value_ptr(transform));
	}
}

void Shader::setMat4f(const string& name, int count, bool transpose, glm::mat4 transform)
{
	if (transpose)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, GL_TRUE, glm::value_ptr(transform));

	}
	else
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, GL_FALSE, glm::value_ptr(transform));
	}
}


