// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_Texcoord;

// Varyings
out vec3 Position;
out vec2 Texcoord;

// Uniforms
uniform mat4 u_proj;

void main()
{
	Position = a_Position;
	Texcoord = a_Texcoord;

	gl_Position = u_proj * vec4(a_Position, 1.0);
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4	gl_Color;
in  vec2	Texcoord;

// Uniforms
uniform sampler2D	u_texture;
uniform vec4		u_color;

void main()
{
	gl_Color = u_color * texture(u_texture, Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * */