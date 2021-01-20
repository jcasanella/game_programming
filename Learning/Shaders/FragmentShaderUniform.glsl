#version 330 core
out vec4 FragColor;

uniform vec4 ourColor;	// input variable from vs (same name and type)

void main()
{
	FragColor = ourColor;
}