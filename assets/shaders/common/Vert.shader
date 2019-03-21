// Vert.shader
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

// Out Vertex
out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} Out;

// Structs
struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};

// Uniforms
uniform Vert_Uniforms Vert;

/* * * * * * * * * * * * * * * * * * * * */