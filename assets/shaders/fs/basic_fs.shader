#version 330 core

layout(location = 0) out vec4 a_color;

uniform vec4 u_color;

void main()
{
	a_color = u_color;
}