#version 330 core

// varyings
out vec4	v_FragColor;
in vec2		v_TexCoords;
in vec4		v_Color;

// uniforms
uniform sampler2D	u_texture;
uniform vec4		u_color;

void main()
{
	float a = texture(u_texture, v_TexCoords).r;

	v_FragColor = vec4(1, 1, 1, a) * u_color;
}