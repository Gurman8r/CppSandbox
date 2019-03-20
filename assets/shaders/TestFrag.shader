#version 410 core

in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} In;

out vec4 FragColor;

struct Frag_Uniforms
{
	vec4 color;
	sampler2D tex0;
};

uniform Frag_Uniforms Frag;

void main()
{
	FragColor = (Frag.color * texture(Frag.tex0, In.Texcoord));
}