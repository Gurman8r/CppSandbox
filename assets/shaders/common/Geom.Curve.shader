// Geom.Curve.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Geom.shader"
#include "../../../assets/shaders/common/Util.Curve.shader"

/* * * * * * * * * * * * * * * * * * * * */

void drawLine(in vec4 p0, in vec4 p1, const int samples, const float dt)
{
	int i = 0;
	float t = 0.0;
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
	float t = 0.0;
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
	float t = 0.0;
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
	float t = 0.0;
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
	float t = 0.0;
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
	int i = 0;
	float t = 0.0;
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
	int i = 0;
	float t = 0.0;

	for (i = 0; i <= samples; i++)
	{
		gl_Position = sampleCubicHermite(p0, m0, p1, m1, t);
		EmitVertex();
		t += dt;
	}
	EndPrimitive();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */