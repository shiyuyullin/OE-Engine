// Vertex shader for all postprocessing

#version 400 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;


void main()
{
    // position the quad on the top right corner of the window
    gl_Position = vec4(aPos.x * 0.3 + 0.7, aPos.y * 0.3 + 0.7, 0.0, 1.0); 
    TexCoords = aTexCoords;

}