// Geom.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Maths
/* * * * * * * * * * * * * * * * * * * * */
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
/* * * * * * * * * * * * * * * * * * * * */
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */