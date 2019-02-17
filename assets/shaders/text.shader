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

void main()
{
	Position	= a_Position;
	Texcoord	= a_Texcoord;

	mat4 mvp = u_proj;

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
	float a = texture(u_texture, Texcoord).r;

	FragColor = vec4(1, 1, 1, a) * u_color;
}

/* * * * * * * * * * * * * * * * * * * * */