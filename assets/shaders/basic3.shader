// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#include "../../../assets/shaders/lib/Vert.shader"

void main()
{
	mat4 mvp = (Vert.proj * Vert.view * Vert.model);
	Out.Position = a_Position;
	Out.Normal = a_Normal;
	Out.Texcoord = a_Texcoord;
	gl_Position = mvp * vec4(a_Position, 1.0);
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#include "../../../assets/shaders/lib/Frag.shader"

void main()
{
	FragColor = (Frag.color * texture(Frag.tex0, In.Texcoord));
}

/* * * * * * * * * * * * * * * * * * * * */