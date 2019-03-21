// lighting.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Vert.MVP.shader>
#shader vertex

void main()
{
	gl_Position = ml_MVP_Position();
	Out.Position = gl_Position.xyz;
	Out.Normal = transpose(inverse(Vert.model)) * Out.Normal;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Frag.Light.shader>
#shader fragment

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	// Ambient
	vec4	ambient = (Frag.ambient * Frag.color);

	// Diffuse 
	vec3	diffNml = normalize(In.Normal.xyz);
	vec3	diffDir = normalize(Frag.position - In.Position);
	float	diffAmt = max(dot(diffNml, diffDir), 0.0);
	vec4	diffCol = vec4(diffAmt * Frag.color.rgb, 1.0);
	vec4	diffTex = texture(Frag.tex_dm, In.Texcoord);
	vec4	diffuse = (diffCol * diffTex);

	// Specular		 
	vec3	specCam = normalize(Frag.camera - In.Position);
	vec3	specRfl = reflect(-diffDir, diffNml);
	float	specAmt = pow(max(dot(specCam, specRfl), 0.0), Frag.shininess);
	vec4	specCol = vec4(Frag.specular * specAmt * Frag.color.rgb, 1.0);
	vec4	specTex = texture(Frag.tex_sm, In.Texcoord);
	vec4	specular= (specCol * specTex);

	gl_Color = (ambient + diffuse + specular);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */