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
	mat4 mvp = (u_proj * u_view * u_model);
	vec4 pos = vec4(a_Position, 1.0);

	FragPos = vec3(mvp * pos);
	Normal = mat3(transpose(inverse(u_model))) * a_Normal;
	Texcoord = a_Texcoord;

	gl_Position = mvp * pos;
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4 FragColor;
in	vec3 FragPos;
in  vec3 Normal;
in  vec2 Texcoord;

// Uniforms
uniform vec4		u_color;
uniform vec3		u_viewPos;
uniform vec3		u_lightPos;
uniform vec4		u_lightCol;
uniform float		u_ambientAmt;
uniform float		u_specularAmt;
uniform int			u_specularPow;
uniform sampler2D	u_tex_dm;
uniform sampler2D	u_tex_sm;

void main()
{
	float alpha = u_color.w;

	// textures
	vec3	tex_dm = texture(u_tex_dm, Texcoord).xyz;
	vec3	tex_sm = texture(u_tex_sm, Texcoord).xyz;

	// ambient
	vec4	ambient = (u_ambientAmt * u_lightCol);

	// diffuse 
	vec3	norm		= normalize(Normal);
	vec3	lightDir	= normalize(u_lightPos - FragPos);
	float	diff		= max(dot(norm, lightDir), 0.0);
	vec3	diffuse		= (diff * u_lightCol.xyz) * tex_dm;

	// specular		 
	vec3	viewDir		= normalize(u_viewPos - FragPos);
	vec3	reflectDir	= reflect(-lightDir, norm);
	float	spec		= pow(max(dot(viewDir, reflectDir), 0.0), u_specularPow);
	vec3	specular	= (u_specularAmt * spec * u_lightCol.xyz) * tex_sm;

	// result
	vec4 result = (ambient) + vec4(diffuse, 1.0) + vec4(specular, 1.0);

	FragColor = result * u_color;
}

/* * * * * * * * * * * * * * * * * * * * */