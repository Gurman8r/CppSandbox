/*
	Pass Attributes World
	By Dan Buckstein
	Vertex shader that passes attributes in world space.
	
	Modified by: Melody Gurman
*/

// version
#version 410


// ****
// attributes
layout (location = 0) in vec4 position;


// ****
// uniforms


// ****
// varyings


// shader function
void main()
{
	// ****
	// set proper clip position
	gl_Position = position;
}