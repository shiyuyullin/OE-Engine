#version 400 core

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 color;
layout (location = 1) in vec2 texCoord;

out vec3 vertexColor;
out vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vertexColor = color;
	textureCoord = texCoord;
}