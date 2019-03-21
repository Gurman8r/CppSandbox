/* * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Vert.shader"
#include "../../../assets/shaders/common/Frag.shader"

/* * * * * * * * * * * * * * * * * * * * */

#shader vertex

void main()
{
	Out.Position = a_Position;
	Out.Normal = a_Normal;
	Out.Texcoord = a_Texcoord;
	gl_Position = (Vert.proj * Vert.view * Vert.model) * vec4(a_Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * */

#shader fragment

void main()
{
	gl_Color = (Frag.color * texture(Frag.tex0, In.Texcoord));
}

/* * * * * * * * * * * * * * * * * * * * */