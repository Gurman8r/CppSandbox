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
	v_Texcoord = a_Texcoord;

	gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
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

// Functions
void drawNormal()
{
	v_FragColor = texture(u_texture, v_Texcoord);
}

void drawInverted()
{
	v_FragColor = vec4(vec3(1.0 - texture(u_texture, v_Texcoord)), 1.0);
}

void drawGrayscale()
{
	v_FragColor = texture(u_texture, v_Texcoord);

	float average = (v_FragColor.r + v_FragColor.g + v_FragColor.b) / 3.0;
	
	v_FragColor = vec4(average, average, average, 1.0);
}

void drawKernel(in float kernel[9])
{
	const float offset = 1.0 / 300.0;

	vec2 offsets[9] = vec2[](
		vec2(-offset,	offset),	// top-left
		vec2(0.0f,		offset),	// top-center
		vec2(offset,	offset),	// top-right
		vec2(-offset,	0.0f),		// center-left
		vec2(0.0f,		0.0f),		// center-center
		vec2(offset,	0.0f),		// center-right
		vec2(-offset,	-offset),	// bottom-left
		vec2(0.0f,		-offset),	// bottom-center
		vec2(offset,	-offset)	// bottom-right    
	);

	vec3 samples[9];
	for (int i = 0; i < 9; i++)
	{
		samples[i] = vec3(texture(u_texture, v_Texcoord.st + offsets[i]));
	}

	vec3 color = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		color += samples[i] * kernel[i];
	}

	v_FragColor = vec4(color, 1.0);
}

// Enums
#define MODE_NORMAL		0
#define MODE_INVERTED	1
#define MODE_GRAYSCALE	2
#define MODE_JUICY		3
#define MODE_BLUR		4

// Main
void main()
{
	const int mode = MODE_JUICY;

	switch (mode)
	{
	case MODE_NORMAL:
		drawNormal();
		break;

	case MODE_INVERTED:
		drawInverted();
		break;

	case MODE_GRAYSCALE:
		drawGrayscale();
		break;

	case MODE_JUICY:
		drawKernel(float[9](
			-1, -1, -1,
			-1, +9, -1,
			-1, -1, -1
		));
		break;

	case MODE_BLUR:
		drawKernel(float[9](
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16
		));
		break;
	};
}

/* * * * * * * * * * * * * * * * * * * * */