// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_Texcoord;

// Varyings
out vec2 Texcoord;

void main()
{
	Texcoord = a_Texcoord;

	gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}


// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4	gl_Color;
in  vec2	Texcoord;

// Uniforms
uniform sampler2D	u_texture;
uniform int			u_mode;

// Functions
void drawNormal()
{
	gl_Color = texture(u_texture, Texcoord);
}

void drawInverted()
{
	gl_Color = vec4(vec3(1.0 - texture(u_texture, Texcoord)), 1.0);
}

void drawGrayscale()
{
	gl_Color = texture(u_texture, Texcoord);

	float average = (gl_Color.r + gl_Color.g + gl_Color.b) / 3.0;
	
	gl_Color = vec4(average, average, average, 1.0);
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
		samples[i] = vec3(texture(u_texture, Texcoord.st + offsets[i]));
	}

	vec3 color = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		color += samples[i] * kernel[i];
	}

	gl_Color = vec4(color, 1.0);
}

// Enums
#define MODE_NORMAL		0
#define MODE_GRAYSCALE	1
#define MODE_BLUR		2
#define MODE_JUICY		3
#define MODE_INVERTED	4

// Main
void main()
{
	switch (u_mode)
	{
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

	case MODE_INVERTED:
		drawInverted();
		break;

	case MODE_NORMAL:
	default:
		drawNormal();
		break;
	};
}

/* * * * * * * * * * * * * * * * * * * * */