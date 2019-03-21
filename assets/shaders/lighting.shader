// lighting.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#include "../../../assets/shaders/common/Vert.MVP.shader"

void main()
{
	gl_Position = ml_Vert_Position();
	Out.Position = vec3(gl_Position);
	Out.Normal = transpose(inverse(Vert.model)) * Out.Normal;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include "../../../assets/shaders/common/Frag.shader"
#include "../../../assets/shaders/common/Util.View.shader"
#include "../../../assets/shaders/common/Util.Light.shader"

/* * * * * * * * * * * * * * * * * * * * */

struct Tex_Uniforms
{
	sampler2D dm;
	sampler2D sm;
};
uniform Tex_Uniforms Tex;

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Ambient
	vec4 ambient = (Light.ambient * Light.color);

	// Diffuse 
	vec3	norm = normalize(In.Normal.xyz);
	vec3	dir = normalize(Light.position - In.Position);
	float	diffAmt = max(dot(norm, dir), 0.0);
	vec3	diffuse = (diffAmt * Light.color.xyz);

	// Specular		 
	vec3	view = normalize(View.position - In.Position);
	vec3	refl = reflect(-dir, norm);
	float	specAmt = pow(max(dot(view, refl), 0.0), Light.shininess);
	vec3	specular = (Light.specular * specAmt * Light.color.xyz);

	// Textures
	vec4 tex_dm = texture(Tex.dm, In.Texcoord);
	vec4 tex_sm = texture(Tex.sm, In.Texcoord);

	gl_Color =
		(ambient) +
		(vec4(diffuse, 1.0) * tex_dm) +
		(vec4(specular, 1.0) * tex_sm);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */