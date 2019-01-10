#version 330 core

in vec3		v_Position;
in vec4		v_Color;
in vec2		v_TexCoords;
out vec4	v_FragColor;

uniform vec4 u_color;

void main()
{
	v_FragColor = u_color * v_Color.w;
}