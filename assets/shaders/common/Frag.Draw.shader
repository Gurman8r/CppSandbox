// Frag.Draw.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include "../../../assets/shaders/common/Frag.shader"

/* * * * * * * * * * * * * * * * * * * * */

struct Frag_Uniforms
{
	vec4 mainCol;
	sampler2D mainTex;
};

/* * * * * * * * * * * * * * * * * * * * */

uniform Frag_Uniforms Frag;

/* * * * * * * * * * * * * * * * * * * * */

vec4 ml_Frag_MainCol()
{
	return Frag.mainCol;
}

vec4 ml_Frag_GetPixel(in vec2 index)
{
	return texture(Frag.mainTex, index);
}

vec4 ml_Frag_MainPixel()
{
	return ml_Frag_GetPixel(In.Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */