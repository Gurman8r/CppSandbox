// Vert.MVP.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#include "../../../assets/shaders/common/Vert.shader"

/* * * * * * * * * * * * * * * * * * * * */

struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};

/* * * * * * * * * * * * * * * * * * * * */

uniform Vert_Uniforms Vert;

/* * * * * * * * * * * * * * * * * * * * */

mat4 ml_Vert_MVP()
{
	return (Vert.proj * Vert.view * Vert.model);
}

vec4 ml_Vert_Position()
{
	ml_Vert_Update();

	return ml_Vert_MVP() * vec4(Out.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * */