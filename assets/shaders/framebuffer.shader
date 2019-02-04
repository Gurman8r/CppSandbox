// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 330 core

// Layout
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;

// Varyings
out vec2 v_Texcoord;

void main()
{
	gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
	v_Texcoord = a_Texcoord;
}

// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 330 core

// Varyings
out vec4	v_FragColor;
in  vec2	v_Texcoord;

// Uniforms
uniform sampler2D u_texture;

void main()
{
	// Normal
	//v_FragColor = texture(u_texture, v_Texcoord);

	// Inverted
	v_FragColor = vec4(vec3(1.0 - texture(u_texture, v_Texcoord)), 1.0);
}

/* * * * * * * * * * * * * * * * * * * * */