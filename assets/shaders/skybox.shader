// skybox.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#include <common/Vert.MVP.shader>

void main()
{
	ml_Vert_Update();

	gl_Position = (Vert.proj * Vert.view) * vec4(Out.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include <common/Frag.Draw.shader>

/* * * * * * * * * * * * * * * * * * * * */

//struct Frag_Uniforms
//{
//	samplerCube	mainTex;
//	vec4 mainCol;
//};
//uniform Frag_Uniforms Frag;

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	gl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */