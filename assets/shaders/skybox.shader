// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Attributes
layout(location = 0) in vec3 a_Position;

// Varyings
out vec3 Texcoord;

// Uniforms
uniform mat4 u_proj;
uniform mat4 u_view;

void main()
{
	Texcoord = a_Position;

	gl_Position = u_proj * u_view * vec4(a_Position, 1.0);
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4 gl_Color;
in	vec3 Texcoord;

// Uniforms
uniform samplerCube	u_skybox;

void main()
{
	gl_Color = texture(u_skybox, Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * */