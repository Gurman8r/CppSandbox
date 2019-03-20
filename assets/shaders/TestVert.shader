#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} Out;

struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};

uniform Vert_Uniforms Vert;

void main()
{
	mat4 mvp = (Vert.proj * Vert.view * Vert.model);
	Out.Position = a_Position;
	Out.Normal = a_Normal;
	Out.Texcoord = a_Texcoord;
	gl_Position = mvp * vec4(a_Position, 1.0);
}