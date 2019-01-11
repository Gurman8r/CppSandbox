/*
	Draw Curve - Morphing
	By Dan Buckstein
	Geometry shader that draws the path that a morphing vertex follows.
	
	Modified by: ______________________________________________________________
*/

#version 410

// *********************************************************************************

// ****
#define SAMPLES_PER_SEGMENT 8
#define SAMPLES_MAX 120

// geometry type layout qualifiers
layout (triangles) in;
layout (line_strip, max_vertices = SAMPLES_MAX) out;

// ****
// receive varying data
in vertexdata
{
	vec4 p0;
	vec4 p1;
	vec4 p2;
	vec4 p3;
} pass[];

// uniforms
uniform mat4 mvp;

// varying output: solid color
out vec4 passColor;

// *********************************************************************************
// LERP
vec4 lerp(in vec4 p0, in vec4 p1, const float t)
{
	return (p1 * t) + (p0 * (1.0f - t));
}

// ****
// Bezier curve interpolation
// easier with recursion... but that's not an option here...
//	...so we'll have to implement specific cases
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

// ****
// Catmull-Rom spline interpolation
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

// ****
// cubic Hermite spline interpolation
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


// ****
// draw line
void drawLine(in vec4 p0, in vec4 p1, const int32_t samples, const float dt)
{
	int32_t i = 0;
	float t = 0.0f;

	for(i = 0; i <= samples; i++)
	{
		gl_Position = mvp * lerp(p0, p1, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}

// ****
// draw entire Bezier curve
void drawBezierCurve0(in vec4 p0, const int32_t samples, const float dt)
{
	int32_t i = 0;
	float t = 0.0f;

	for(i = 0; i <= samples; i++)
	{
		gl_Position = mvp * sampleBezier0(p0, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawBezierCurve1(in vec4 p0, in vec4 p1, const int32_t samples, const float dt)
{
	int32_t i = 0;
	float t = 0.0f;

	for(i = 0; i <= samples; i++)
	{
		gl_Position = mvp * sampleBezier1(p0, p1, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawBezierCurve2(in vec4 p0, in vec4 p1, in vec4 p2, const int32_t samples, const float dt)
{
	int32_t i = 0;
	float t = 0.0f;

	for(i = 0; i <= samples; i++)
	{
		gl_Position = mvp * sampleBezier2(p0, p1, p2, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}
void drawBezierCurve3(in vec4 p0, in vec4 p1, in vec4 p2, in vec4 p3, const int32_t samples, const float dt)
{
	int32_t i = 0;
	float t = 0.0f;

	for(i = 0; i <= samples; i++)
	{
		gl_Position = mvp * sampleBezier3(p0, p1, p2, p3, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}

// ****
// draw Catmull-Rom spline segment
void drawCatmullRomSplineSegment(in vec4 pPrev, in vec4 p0, in vec4 p1, in vec4 pNext, const int32_t samples, const float dt)
{
	// vec4 sampleCatmullRom(in vec4 pPrev, in vec4 p0, in vec4 p1, in vec4 pNext, const float t)

	int32_t i = 0;
	float t = 0.0f;

	for(i = 0; i <= samples; i++)
	{
		gl_Position = mvp * sampleCatmullRom(pPrev, p0, p1, pNext, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}

// ****
// draw cubic Hermite spline segment
void drawCubicHermiteSplineSegment(in vec4 p0, in vec4 m0, in vec4 p1, in vec4 m1, const int32_t samples, const float dt)
{
	// vec4 sampleCubicHermite(in vec4 p0, in vec4 m0, in vec4 p1, in vec4 m1, const float t)

	int32_t i = 0;
	float t = 0.0f;

	for(i = 0; i <= samples; i++)
	{
		gl_Position = mvp * sampleCubicHermite(p0, m0, p1, m1, t);

		EmitVertex();

		t += dt;
	}

	EndPrimitive();
}

void drawLineFull(in vec4 p0, in vec4 p1)
{
	gl_Position = mvp * p0;
	EmitVertex();
	gl_Position = mvp * p1;
	EmitVertex();
	EndPrimitive();
}

void drawCurve(in vec4 p0, in vec4 p1, in vec4 p2, in vec4 p3)
{
}

// *********************************************************************************

void main()
{
	

	const int32_t samples = SAMPLES_PER_SEGMENT;
	const float dt = 1.0 / float(SAMPLES_PER_SEGMENT);
	
	passColor = vec4(0.5f, 0.0f, 1.0f, 1.0f);

	drawCatmullRomSplineSegment(
		pass[0].p0, 
		pass[0].p1, 
		pass[0].p2, 
		pass[0].p3, 
		SAMPLES_PER_SEGMENT, dt);
	drawCatmullRomSplineSegment(
		pass[1].p0, 
		pass[1].p1, 
		pass[1].p2, 
		pass[1].p3, 
		SAMPLES_PER_SEGMENT, dt);
	drawCatmullRomSplineSegment(
		pass[2].p0, 
		pass[2].p1, 
		pass[2].p2, 
		pass[2].p3, 
		SAMPLES_PER_SEGMENT, dt);

	drawCatmullRomSplineSegment(
		pass[0].p3, 
		pass[0].p0, 
		pass[0].p1, 
		pass[0].p2, 
		SAMPLES_PER_SEGMENT, dt);
	drawCatmullRomSplineSegment(
		pass[1].p3, 
		pass[1].p0, 
		pass[1].p1, 
		pass[1].p2, 
		SAMPLES_PER_SEGMENT, dt);
	drawCatmullRomSplineSegment(
		pass[2].p3, 
		pass[2].p0, 
		pass[2].p1, 
		pass[2].p2, 
		SAMPLES_PER_SEGMENT, dt);

	drawCatmullRomSplineSegment(
		pass[0].p2, 
		pass[0].p3, 
		pass[0].p0, 
		pass[0].p1, 
		SAMPLES_PER_SEGMENT, dt);
	drawCatmullRomSplineSegment(
		pass[1].p2, 
		pass[1].p3, 
		pass[1].p0, 
		pass[1].p1, 
		SAMPLES_PER_SEGMENT, dt);
	drawCatmullRomSplineSegment(
		pass[2].p2, 
		pass[2].p3, 
		pass[2].p0, 
		pass[2].p1, 
		SAMPLES_PER_SEGMENT, dt);

	drawCatmullRomSplineSegment(
		pass[0].p1, 
		pass[0].p2, 
		pass[0].p3, 
		pass[0].p0, 
		SAMPLES_PER_SEGMENT, dt);
	drawCatmullRomSplineSegment(
		pass[1].p1, 
		pass[1].p2, 
		pass[1].p3, 
		pass[1].p0, 
		SAMPLES_PER_SEGMENT, dt);
	drawCatmullRomSplineSegment(
		pass[2].p1, 
		pass[2].p2, 
		pass[2].p3, 
		pass[2].p0, 
		SAMPLES_PER_SEGMENT, dt);

	
}

/*
	drawLineFull(pass[0].p0, pass[0].p1);
	drawLineFull(pass[1].p0, pass[1].p1);
	drawLineFull(pass[2].p0, pass[2].p1);

	drawLineFull(pass[0].p1, pass[0].p2);
	drawLineFull(pass[1].p1, pass[1].p2);
	drawLineFull(pass[2].p1, pass[2].p2);

	drawLineFull(pass[0].p2, pass[0].p3);
	drawLineFull(pass[1].p2, pass[1].p3);
	drawLineFull(pass[2].p2, pass[2].p3);

	drawLineFull(pass[0].p3, pass[0].p0);
	drawLineFull(pass[1].p3, pass[1].p0);
	drawLineFull(pass[2].p3, pass[2].p0);
*/