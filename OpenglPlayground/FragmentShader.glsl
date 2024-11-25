#version 400 core

in vec3 vertexColor;
in vec2 textureCoord;

out vec4 FragColor;

uniform sampler2D wood;
uniform sampler2D laughFace;

uniform float blendIntensity;

void main()
{

	FragColor = mix(texture(wood, textureCoord), texture(laughFace, textureCoord), blendIntensity);

}