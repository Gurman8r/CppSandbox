// Frag.Light.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include <common/Frag.shader>

struct Frag_Uniforms
{
	vec3		camera;
	vec3		position;
	vec4		color;
	float		ambient;
	float		specular;
	int			shininess;
	sampler2D	tex_dm;
	sampler2D	tex_sm;
};
uniform Frag_Uniforms Frag;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */