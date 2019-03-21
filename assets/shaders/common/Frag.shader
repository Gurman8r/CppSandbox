// Frag.shader
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// In Vertex
in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} In;

// Frag Color
out vec4 gl_Color;

// Structs
struct Frag_Uniforms
{
	vec4 color;
	sampler2D mainTex;
};

// Uniforms
uniform Frag_Uniforms Frag;

/* * * * * * * * * * * * * * * * * * * * */