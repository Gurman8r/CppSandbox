/*
	Skinning & Pass Normal as Color
	By Dan Buckstein
	Vertex shader that performs vertex skinning, passing resulting normal out
		as a solid color for debugging purposes.
	
	Modified by: Melody Gurman
*/

#version 450


// ****
// the usual attributes, but also need skinning attributes: 
// weights and bone indices
layout (location = 0) in vec4 position;
layout (location = 2) in vec4 normal;
layout (location = 1) in vec4 blendWeight;
layout (location = 7) in vec4 blendIndex;

uniform mat4 mvp;


#define BONES_MAX 64

// ****
// skinning uniforms
uniform mat4 bones_skinning[BONES_MAX];


// pass some piece of data along for debugging
out vec4 passColor;


// ****
// skinning algorithm: 
// vector goes indeformed vector comes out
vec4 skin(in vec4 v)
{
	vec4 v_out = vec4(0.0);

	v_out += bones_skinning[int32_t(blendIndex.x)] * blendWeight.x * v; 
	v_out += bones_skinning[int32_t(blendIndex.y)] * blendWeight.y * v; 
	v_out += bones_skinning[int32_t(blendIndex.z)] * blendWeight.z * v; 
	v_out += bones_skinning[int32_t(blendIndex.w)] * blendWeight.w * v; 

	return v_out;
}


void main()
{
	// ****
	// skin any attribute that has to do with the SURFACE
	// i.e. anything that could potentially affect shading

	gl_Position = mvp * skin(position);

	// pass debug color
	passColor = 0.5f + (skin(normal) * 0.5f);
}