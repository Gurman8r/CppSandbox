// framebuffer.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#include <common/Vert.MVP.shader>

void main()
{
	ml_Vert_Update();

	gl_Position = vec4(Out.Position.x, Out.Position.y, 0.0, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include <common/Frag.Draw.shader>

#define MODE_NORMAL		0
#define MODE_GRAYSCALE	1
#define MODE_BLUR		2
#define MODE_JUICY		3
#define MODE_INVERTED	4

/* * * * * * * * * * * * * * * * * * * * */

struct Effect_Uniforms
{
	int mode;
};
uniform Effect_Uniforms Effect;

/* * * * * * * * * * * * * * * * * * * * */

vec4 drawInverted()
{
	return vec4(vec3(1.0 - ml_Frag_MainTex()), 1.0);
}

vec4 drawGrayscale()
{
	vec4 temp = ml_Frag_MainTex();

	float average = (temp.r + temp.g + temp.b) / 3.0;

	return vec4(average, average, average, 1.0);
}

vec4 drawKernel(in float kernel[9])
{
	const float offset = 1.0 / 300.0;

	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	// top-left
		vec2(0.0f, offset),		// top-center
		vec2(offset, offset),	// top-right
		vec2(-offset, 0.0f),	// center-left
		vec2(0.0f, 0.0f),		// center-center
		vec2(offset, 0.0f),		// center-right
		vec2(-offset, -offset),	// bottom-left
		vec2(0.0f, -offset),	// bottom-center
		vec2(offset, -offset)	// bottom-right    
	);

	vec3 samples[9];
	for (int i = 0; i < 9; i++)
	{
		samples[i] = vec3(ml_Frag_GetPixel(In.Texcoord.st + offsets[i]));
	}

	vec3 color = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		color += samples[i] * kernel[i];
	}

	return vec4(color, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	switch (Effect.mode)
	{
	case MODE_GRAYSCALE:
		gl_Color = drawGrayscale();
		break;

	case MODE_JUICY:
		gl_Color = drawKernel(float[9](
			-1, -1, -1,
			-1, +9, -1,
			-1, -1, -1
			));
		break;

	case MODE_BLUR:
		gl_Color = drawKernel(float[9](
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16
			));
		break;

	case MODE_INVERTED:
		gl_Color = drawInverted();
		break;

	case MODE_NORMAL:
	default:
		gl_Color = ml_Frag_MainTex();
		break;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */