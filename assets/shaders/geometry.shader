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

void main()
{
	v_Position = a_Position;

	gl_Position = vec4(a_Position, 1.0);
}


// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4 v_FragColor;

// Uniforms
uniform vec4 u_color;

void main()
{
	v_FragColor = u_color;
}


// Geometry
/* * * * * * * * * * * * * * * * * * * * */
#shader geometry
#version 410 core

// Constants
#define SAMPLES_PER_SEGMENT 16
#define SAMPLES_MAX 128

#define CURVE_LINES 0
#define CURVE_BEZIER 1
#define CURVE_CATMULLROM 2
#define CURVE_HERMITE 3

#define RED		vec4(1.0, 0.5, 0.0, 1.0)
#define BLUE	vec4(0.0, 0.5, 1.0, 1.0)
#define GREEN	vec4(0.0, 1.0, 0.5, 1.0)

// Layout
layout(points) in;
layout(line_strip, max_vertices = SAMPLES_MAX) out;

// Uniforms
uniform int		u_curveMode;
uniform float	u_delta;
uniform int		u_samples;

// Maths
vec4 lerp(in vec4 p0, in vec4 p1, const float t)
{
	return (p1 * t) + (p0 * (1.0f - t));
}
vec4 sampleBezier0(in vec4 p0, const float t)
{
	return p0;
}
vec4 sampleBezier1(in vec4 p0, in vec4 p1, const float t)
{
	return lerp(p0, p1, t);
}
vec4 sampleBezier2(in vec4 p0, in vec4 p1, in vec4 p2, const float t)
{
	return lerp(lerp(p0, p1, t), lerp(p1, p2, t), t);
}
vec4 sampleBezier3(in vec4 p0, in vec4 p1, in vec4 p2, in vec4 p3, const float t)
{
	return lerp(sampleBezier2(p0, p1, p2, t), sampleBezier2(p1, p2, p3, t), t);
}
vec4 sampleCatmullRom(in vec4 pPrev, in vec4 p0, in vec4 p1, in vec4 pNext, const float t)
{
	mat4 a = mat4(pPrev, p0, p1, pNext);

	mat4 b = mat4(
		0.0f, -1.0f, 2.0f, -1.0f,
		2.0f, 0.0f, -5.0f, 3.0f,
		0.0f, 1.0f, 4.0f, -3.0f,
		0.0f, 0.0f, -1.0f, 1.0f) * 0.5f;
	b = transpose(b);

	vec4 c = vec4(1.0f, t, t * t, t * t * t);

	return a * b * c;
}
vec4 sampleCubicHermite(in vec4 p0, in vec4 m0, in vec4 p1, in vec4 m1, const float t)
{
	mat4 a = mat4(p0, m0, p1, m1);

	mat4 b = mat4(
		1.0f, 0.0f, -3.0f, 2.0f,
		0.0f, 1.0f, -2.0f, 1.0f,
		0.0f, 0.0f, 3.0f, -2.0f,
		0.0f, 0.0f, -1.0f, 1.0f);
	b = transpose(b);

	vec4 c = vec4(1.0f, t, t * t, t * t * t);

	return a * b * c;
}

// Drawing
void drawLine(in vec4 p0, in vec4 p1, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0f;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = lerp(p0, p1, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawLineFull(in vec4 p0, in vec4 p1)
{
	gl_Position = p0;
	EmitVertex();
	gl_Position = p1;
	EmitVertex();
	EndPrimitive();
}
void drawBezierCurve0(in vec4 p0, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0f;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier0(p0, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawBezierCurve1(in vec4 p0, in vec4 p1, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0f;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier1(p0, p1, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawBezierCurve2(in vec4 p0, in vec4 p1, in vec4 p2, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0f;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier2(p0, p1, p2, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawBezierCurve3(in vec4 p0, in vec4 p1, in vec4 p2, in vec4 p3, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0f;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleBezier3(p0, p1, p2, p3, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawCatmullRomSplineSegment(in vec4 pPrev, in vec4 p0, in vec4 p1, in vec4 pNext, const int samples, const float dt)
{
	// vec4 sampleCatmullRom(in vec4 pPrev, in vec4 p0, in vec4 p1, in vec4 pNext, const float t)

	int i = 0;
	float t = 0.0f;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleCatmullRom(pPrev, p0, p1, pNext, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawCubicHermiteSplineSegment(in vec4 p0, in vec4 m0, in vec4 p1, in vec4 m1, const int samples, const float dt)
{
	// vec4 sampleCubicHermite(in vec4 p0, in vec4 m0, in vec4 p1, in vec4 m1, const float t)

	int i = 0;
	float t = 0.0f;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleCubicHermite(p0, m0, p1, m1, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}

// Stub
void stub(in int samples, in float dt)
{
	// test points
	vec4 testP0 = vec4(-0.5, 0.5, 0.0, 1.0);
	vec4 testP1 = vec4(0.5, 0.5, 0.0, 1.0);
	vec4 testP2 = vec4(0.5, -0.5, 0.0, 1.0);
	vec4 testP3 = vec4(-0.5, -0.5, 0.0, 1.0);

	vec4 p0, p1, p2, p3, pPrev, pNext, m0, m1;

	switch (u_curveMode)
	{
	case CURVE_LINES:
		// multiple segments
		drawLine(testP0, testP1, samples, dt);
		drawLine(testP1, testP2, samples, dt);
		drawLine(testP2, testP3, samples, dt);
		drawLine(testP3, testP0, samples, dt);
		break;
	case CURVE_BEZIER:
		// full curve
		drawBezierCurve3(testP0, testP1, testP2, testP3, samples, dt);
		break;
	case CURVE_CATMULLROM:
		// curve segment
		pPrev = testP0;
		p0 = testP1;
		p1 = testP2;
		pNext = testP3;
		drawCatmullRomSplineSegment(pPrev, p0, p1, pNext, samples, dt);
		break;
	case CURVE_HERMITE:
		// curve segment
		p0 = testP0;
		m0 = testP1 - p0;
		p1 = testP2;
		m1 = testP3 - p1;
		drawCubicHermiteSplineSegment(p0, m0, p1, m1, samples, dt);
		// tangents
		drawLineFull(p0, p0 + m0);
		drawLineFull(p1, p1 + m1);
		break;
	};
}

// Main
/* * * * * * * * * * * * * * * * * * * * */
void main()
{
	float dt = u_delta / float(u_samples);

	stub(u_samples, dt);
}