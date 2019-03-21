// Frag.Draw.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include <common/Frag.shader>

/* * * * * * * * * * * * * * * * * * * * */

struct Frag_Uniforms
{
	vec4 mainCol;
	sampler2D mainTex;
};
uniform Frag_Uniforms Frag;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */