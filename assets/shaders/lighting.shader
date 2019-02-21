// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Layout
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

// Varyings
out vec3 FragPos;
out vec3 Normal;
out vec2 Texcoord;

// Uniforms
uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	mat4 mvp	= (u_proj * u_view * u_model);
	vec4 pos	= mvp * vec4(a_Position, 1.0);

	FragPos		= vec3(pos);
	Normal		= mat3(transpose(inverse(u_model))) * a_Normal;
	Texcoord	= a_Texcoord;

	gl_Position = pos;
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
in	vec3 FragPos;
in  vec3 Normal;
in  vec2 Texcoord;
out vec4 FragColor;

// Uniforms
uniform vec3		u_viewPos;
uniform sampler2D	u_tex_dm;
uniform sampler2D	u_tex_sm;
uniform vec3		u_lightPos;
uniform vec4		u_lightCol;
uniform float		u_ambient;
uniform float		u_specular;
uniform int			u_shininess;

void main()
{
	// ambient
	vec3	ambient	= (u_ambient * u_lightCol.xyz);

	// diffuse 
	vec3	norm	= normalize(Normal);
	vec3	lightDir= normalize(u_lightPos - FragPos);
	float	diff	= max(dot(norm, lightDir), 0.0);
	vec3	diffuse	= (diff * u_lightCol.xyz);

	// specular		 
	vec3	view	= normalize(u_viewPos - FragPos);
	vec3	refl	= reflect(-lightDir, norm);
	float	spec	= pow(max(dot(view, refl), 0.0), u_shininess);
	vec3	specular= (u_specular * spec * u_lightCol.xyz);

	// textures
	vec4	tex_dm	= texture(u_tex_dm, Texcoord);
	vec4	tex_sm	= texture(u_tex_sm, Texcoord);

	// result
	FragColor =
		(vec4(ambient, 1.0)) +
		(vec4(diffuse, 1.0) * tex_dm) +
		(vec4(specular, 1.0) * tex_sm);
}

/* * * * * * * * * * * * * * * * * * * * */