/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#version 410 core

// In Data
/* * * * * * * * * * * * * * * * * * * * */
in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} In;

// Out Data
/* * * * * * * * * * * * * * * * * * * * */
out vec4 FragColor;

// Uniforms
/* * * * * * * * * * * * * * * * * * * * */
struct Frag_Uniforms
{
	vec4 color;
	sampler2D tex0;
};

uniform Frag_Uniforms Frag;

// Main
/* * * * * * * * * * * * * * * * * * * * */
void main()
{
	FragColor = (Frag.color * texture(Frag.tex0, In.Texcoord));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */