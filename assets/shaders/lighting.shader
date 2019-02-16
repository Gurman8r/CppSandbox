// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Layout
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;

// Varyings
out vec3 v_Position;
out vec4 v_Color;
out vec2 v_Texcoord;

// Uniforms
uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	v_Position = vec3(u_model * vec4(a_Position, 1.0));
	v_Color = a_Color;
	v_Texcoord = a_Texcoord;

	mat4 mvp = (u_proj * u_view * u_model);

	gl_Position = mvp * vec4(a_Position, 1.0);
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4	v_FragColor;
in	vec3	v_Position;
in  vec4	v_Color;
in  vec2	v_Texcoord;

// Uniforms
uniform sampler2D	u_texture;
uniform vec4		u_color;
uniform vec3		u_lightPos;
uniform vec4		u_lightCol;
uniform float		u_ambient;

void main()
{
	vec3 ambient	= (u_ambient * u_lightCol.xyz);
	vec3 norm		= normalize(v_Color.xyz);
	vec3 dir		= normalize(u_lightPos - v_Position);
	float diff		= max(dot(norm, dir), 0.0);
	vec3 diffuse	= (diff * u_lightCol.xyz);
	vec4 result		= ((vec4(ambient, 1.0) * vec4(diffuse, 1.0)) * u_color);
	vec4 tex_dm		= texture(u_texture, v_Texcoord);

	v_FragColor = result * tex_dm;

	//v_FragColor = u_color * v_Color * texture(u_texture, v_Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * */