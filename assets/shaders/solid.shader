// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Attributes
layout(location = 0) in vec3 a_Position;

// Uniforms
uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	mat4 mvp = (u_proj * u_view * u_model);

	gl_Position = mvp * vec4(a_Position, 1.0);
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4 gl_Color;

// Uniforms
uniform vec4 u_color;

void main()
{
	gl_Color = u_color;
}

/* * * * * * * * * * * * * * * * * * * * */