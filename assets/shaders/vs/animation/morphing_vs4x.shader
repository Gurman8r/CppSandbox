/*
	Morphing
	By Dan Buckstein
	Vertex shader that performs morph target animation.
	
	Modified by: ______________________________________________________________
*/

#version 450


// ****
// attributes: one per morphing attribute (e.g. multiple positions)
layout (location = 0) in vec4 p0;
layout (location = 1) in vec4 p1;
layout (location = 2) in vec4 p2;
layout (location = 3) in vec4 p3;

// uniforms: animation controls
uniform mat4	mvp;

#define KEYFRAME_COUNT 4
uniform float	param;
uniform int index[KEYFRAME_COUNT];

// varying output: solid color
out vec4 passColor;


// *********************************************************************************

// ****
// LERP
vec4 lerp(in vec4 p0, in vec4 p1, const float t)
{
	return (p1 * t) + (p0 * (1.0f - t));
}

// ****
// Bezier curve interpolation
vec4 Bezier0(in vec4 p0, const float t)
{
	return p0;
}
vec4 Bezier1(in vec4 p0, in vec4 p1, const float t)
{
	return lerp(p0, p1, t);
}
vec4 Bezier2(in vec4 p0, in vec4 p1, in vec4 p2, const float t)
{
	return lerp(lerp(p0, p1, t), lerp(p1, p2, t), t);
}
vec4 Bezier3(in vec4 p0, in vec4 p1, in vec4 p2, in vec4 p3, const float t)
{
	return lerp(Bezier2(p0, p1, p2, t), Bezier2(p1, p2, p3, t), t);
}

// ****
// Catmull-Rom spline interpolation
vec4 CatmullRom(in vec4 pPrev, in vec4 p0, in vec4 p1, in vec4 pNext, const float t)
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
vec4 CubicHermite(in vec4 p0, in vec4 m0, in vec4 p1, in vec4 m1, const float t)
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

// *********************************************************************************

void main()
{
	vec4 position;
	vec4 p[KEYFRAME_COUNT] = { p0, p1, p2, p3 };

	//position = lerp(p[index[0]], p[index[1]], param);
	position = CatmullRom(p[index[0]], p[index[1]], p[index[2]], p[index[3]], param);

	//position = Bezier3(p[index[0]], p[index[1]], p[index[2]], p[index[3]], param);
	//position = CubicHermite(p[index[0]], p[index[1]], p[index[2]], p[index[3]], param);

	// ****
	// do morphing, transform the correct result before assigning to output
	gl_Position = mvp * position;

	// TESTING: send position as color to give us some variance
	passColor = position*0.5+0.5;
}