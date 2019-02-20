// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Layout
layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_Texcoord;

// Varyings
out vec3 Position;
out vec2 Texcoord;

// Uniforms
uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	Position	= a_Position;
	Texcoord	= a_Texcoord;

	mat4 mvp = (u_proj * u_view * u_model);

	gl_Position = mvp * vec4(a_Position, 1.0);
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4	FragColor;
in  vec2	Texcoord;

// Uniforms
uniform sampler2D	u_texture;
uniform vec4		u_color;

void main()
{
	FragColor = u_color * texture(u_texture, Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * */