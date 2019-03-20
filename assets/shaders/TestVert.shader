/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#version 410 core

// Layout
/* * * * * * * * * * * * * * * * * * * * */
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

// Out Data
/* * * * * * * * * * * * * * * * * * * * */
out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} Out;

// Uniforms
/* * * * * * * * * * * * * * * * * * * * */
struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};

uniform Vert_Uniforms Vert;

// Main
/* * * * * * * * * * * * * * * * * * * * */
void main()
{
	Out.Position = a_Position;
	Out.Normal	 = a_Normal;
	Out.Texcoord = a_Texcoord;
	gl_Position  = (Vert.proj * Vert.view * Vert.model) * vec4(a_Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */