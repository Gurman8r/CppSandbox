// Util.Light.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct Light_Uniforms
{
	vec3	position;
	vec4	color;
	float	ambient;
	float	specular;
	int		shininess;
};

/* * * * * * * * * * * * * * * * * * * * */

uniform Light_Uniforms Light;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */