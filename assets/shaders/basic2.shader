// basic2.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#version 410 core

// Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

// Vertex Data
out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} Out;

// Uniforms
struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};
uniform Vert_Uniforms Vert;

// Main
void main()
{
	mat4 mvp = (Vert.proj * Vert.view * Vert.model);
	Out.Position = a_Position;
	Out.Normal = a_Normal;
	Out.Texcoord = a_Texcoord;
	gl_Position = mvp * vec4(a_Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#version 410 core

// Vertex Data
in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} In;

// Frag Color
out vec4 gl_Color;

// Uniforms
struct Frag_Uniforms
{
	vec4 mainCol;
	sampler2D mainTex;
};
uniform Frag_Uniforms Frag;

// Main
void main()
{
	gl_Color = (Frag.mainCol * texture(Frag.mainTex, In.Texcoord));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */