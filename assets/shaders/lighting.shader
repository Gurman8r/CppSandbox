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
	v_Position = a_Position;
	v_Texcoord = a_Texcoord;
	v_Color = a_Color;

	mat4 mvp = (u_proj * u_view * u_model);

	gl_Position = mvp * vec4(a_Position, 1.0);
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4	v_FragColor;
in  vec3	v_Position;
in  vec2	v_Texcoord;
in  vec4	v_Color;

// Uniforms
uniform sampler2D u_texture;
uniform vec4 u_color;
uniform vec3 u_lightPos;
uniform vec4 u_lightColor;

void main()
{
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_lightColor;
	// diffuse 
	//vec4 norm = normalize(v_Color);
	//vec4 lightDir = vec4(normalize(u_lightPos - v_Position), 1);
	//float diff = max(dot(norm, lightDir), 0.0);
	//vec4 diffuse = diff * u_lightColor;
	//vec4 result = (ambient + diffuse) * u_color;
	//v_FragColor = vec4(result, 1.0);

	v_FragColor = u_color * v_Color * texture(u_texture, v_Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * */