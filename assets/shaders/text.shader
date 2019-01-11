/* Vertex
* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 330 core

// attributes
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texcoord;

// varyings
out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoords;

// uniforms
uniform mat4 u_proj;

void main()
{
	v_Position = a_position;
	v_TexCoords = a_texcoord;
	v_Color = a_color;

	gl_Position = u_proj * vec4(a_position, 1.0);
}

/* Fragment
* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 330 core

// varyings
out vec4	v_FragColor;
in vec2		v_TexCoords;
in vec4		v_Color;

// uniforms
uniform sampler2D	u_texture;
uniform vec4		u_color;

void main()
{
	float a = texture(u_texture, v_TexCoords).r;

	v_FragColor = vec4(1, 1, 1, a) * u_color;
}

/* * * * * * * * * * * * * * * * * * * * */