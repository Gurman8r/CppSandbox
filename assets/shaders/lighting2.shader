// Vertex
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Attributes
/* * * * * * * * * * * * * * * * * * * * */
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

// Out Data
/* * * * * * * * * * * * * * * * * * * * */
out VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} Out;

// Uniforms
/* * * * * * * * * * * * * * * * * * * * */
struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};
uniform Vert_Uniforms Vert;

// Program
/* * * * * * * * * * * * * * * * * * * * */
vec4 vertOutput()
{
	mat4 mvp = (Vert.proj * Vert.view * Vert.model);
	vec4 pos = mvp * vec4(a_Position, 1.0);
	Out.Position = vec3(pos);
	Out.Normal	 = vec4(mat3(transpose(inverse(Vert.model))) * a_Normal, 1.0);
	Out.Texcoord = a_Texcoord;

	return pos;
}

// Main
/* * * * * * * * * * * * * * * * * * * * */
void main()
{
	gl_Position = vertOutput();
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// In Data
/* * * * * * * * * * * * * * * * * * * * */
in VertexData
{
	vec3 Position;
	vec4 Normal;
	vec2 Texcoord;
} In;

// Out Data
/* * * * * * * * * * * * * * * * * * * * */
out vec4 gl_Color;

// Uniforms
/* * * * * * * * * * * * * * * * * * * * */
struct View_Uniforms
{
	vec3 position;
};
struct Tex_Uniforms
{
	sampler2D dm;
	sampler2D sm;
};
struct Light_Uniforms
{
	vec3	position;
	vec4	color;
	float	ambient;
	float	specular;
	int		shininess;
};

uniform View_Uniforms	View;
uniform Tex_Uniforms	Tex;
uniform Light_Uniforms	Light;

// Program
/* * * * * * * * * * * * * * * * * * * * */
vec4 fragOutput()
{
	// Ambient
	vec4 ambient = (Light.ambient * Light.color);

	// Diffuse 
	vec3	norm	= normalize(In.Normal.xyz);
	vec3	dir		= normalize(Light.position - In.Position);
	float	diffAmt	= max(dot(norm, dir), 0.0);
	vec3	diffuse = (diffAmt * Light.color.xyz);

	// Specular		 
	vec3	view	= normalize(View.position - In.Position);
	vec3	refl	= reflect(-dir, norm);
	float	specAmt	= pow(max(dot(view, refl), 0.0), Light.shininess);
	vec3	specular= (Light.specular * specAmt * Light.color.xyz);

	// Textures
	vec4 tex_dm	= texture(Tex.dm, In.Texcoord);
	vec4 tex_sm	= texture(Tex.sm, In.Texcoord);

	return
		(ambient) +
		(vec4(diffuse, 1.0) * tex_dm) +
		(vec4(specular, 1.0) * tex_sm);
}

// Main
/* * * * * * * * * * * * * * * * * * * * */
void main()
{
	gl_Color = fragOutput();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */