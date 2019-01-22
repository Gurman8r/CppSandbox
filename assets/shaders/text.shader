/* Vertex
* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 330 core

// Layout
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;

// Varyings
out vec3 v_Position;
out vec4 v_Color;
out vec2 v_Texcoord;

// Uniforms
uniform mat4 u_proj;

void main()
{
	v_Position	= a_Position;
	v_Texcoord = a_Texcoord;
	v_Color		= a_Color;

	gl_Position = u_proj * vec4(a_Position, 1.0);
}

/* Fragment
* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 330 core

// Varyings
out vec4	v_FragColor;
in  vec2	v_Texcoord;
in  vec4	v_Color;

// Uniforms
uniform sampler2D	u_texture;
uniform vec4		u_color;

void main()
{
	float a = texture(u_texture, v_Texcoord).r;

	v_FragColor = vec4(1, 1, 1, a) * u_color;
}

/* * * * * * * * * * * * * * * * * * * * */