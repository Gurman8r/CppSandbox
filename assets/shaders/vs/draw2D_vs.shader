#version 330 core

// attributes
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_texcoord;

// varyings
out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoords;

// uniforms
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    v_Position	= a_position;
	v_TexCoords	= a_texcoord;
	v_Color		= a_color;

	mat4 mvp = u_proj * u_view * u_model;

    gl_Position = mvp * vec4(a_position, 1.0);
}