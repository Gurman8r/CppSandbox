// Vertex
/* * * * * * * * * * * * * * * * * * * * */
#shader vertex
#version 410 core

// Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

// Varyings
out vec3 Position;

void main()
{
	Position = a_Position;

	gl_Position = vec4(a_Position, 1.0);
}


// Fragment
/* * * * * * * * * * * * * * * * * * * * */
#shader fragment
#version 410 core

// Varyings
out vec4 FragColor;

// Uniforms
uniform vec4 u_color;

void main()
{
	FragColor = u_color;
}


// Geometry
/* * * * * * * * * * * * * * * * * * * * */
#shader geometry
#version 410 core
#include "../../../assets/shaders/common/Geom.shader"

// Macros
#define SAMPLES_PER_SEGMENT 16
#define SAMPLES_MAX 128

#define CURVE_LINES 0
#define CURVE_BEZIER 1
#define CURVE_CATMULLROM 2
#define CURVE_HERMITE 3

#define RED		vec4(1.0, 0.5, 0.0, 1.0)
#define BLUE	vec4(0.0, 0.5, 1.0, 1.0)
#define GREEN	vec4(0.0, 1.0, 0.5, 1.0)

// Attributes
layout(points) in;
layout(line_strip, max_vertices = SAMPLES_MAX) out;

// Uniforms
uniform int		u_lineMode;
uniform float	u_lineDelta;
uniform float	u_lineSize;
uniform int		u_lineSamples;

// Stub
void stub(in int samples, in float dt)
{
	// test points
	float size = u_lineSize;
	vec4 testP0 = vec4(-size, +size, 0.0, 1.0);
	vec4 testP1 = vec4(+size, +size, 0.0, 1.0);
	vec4 testP2 = vec4(+size, -size, 0.0, 1.0);
	vec4 testP3 = vec4(-size, -size, 0.0, 1.0);

	vec4 p0, p1, p2, p3, pPrev, pNext, m0, m1;

	switch (u_lineMode)
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

	default:
		drawLineFull(testP0, testP1);
		drawLineFull(testP1, testP2);
		drawLineFull(testP2, testP3);
		drawLineFull(testP3, testP0);
		break;
	};
}

// Main
/* * * * * * * * * * * * * * * * * * * * */
void main()
{
	float dt = u_lineDelta / float(u_lineSamples);

	stub(u_lineSamples, dt);
}