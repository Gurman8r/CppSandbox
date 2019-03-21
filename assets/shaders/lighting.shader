// lighting.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#include <common/Vert.MVP.shader>

void main()
{
	gl_Position = ml_MVP_Position();
	Out.Position = gl_Position.xyz;
	Out.Normal = transpose(inverse(Vert.model)) * Out.Normal;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include <common/Frag.Light.shader>

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Ambient
	vec4 ambient = (Frag.ambient * Frag.color);

	// Diffuse 
	vec3	norm = normalize(In.Normal.xyz);
	vec3	dir = normalize(Frag.position - In.Position);
	float	diffAmt = max(dot(norm, dir), 0.0);
	vec3	diffuse = (diffAmt * Frag.color.rgb);

	// Specular		 
	vec3	view = normalize(Frag.camera - In.Position);
	vec3	refl = reflect(-dir, norm);
	float	specAmt = pow(max(dot(view, refl), 0.0), Frag.shininess);
	vec3	specular = (Frag.specular * specAmt * Frag.color.rgb);

	// Textures
	vec4 tex_dm = texture(Frag.tex_dm, In.Texcoord);
	vec4 tex_sm = texture(Frag.tex_sm, In.Texcoord);

	gl_Color =
		(ambient) +
		(vec4(diffuse, 1.0) * tex_dm) +
		(vec4(specular, 1.0) * tex_sm);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */