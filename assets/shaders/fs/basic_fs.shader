#version 330 core

// varyings
out vec4	v_FragColor;
in vec2		v_TexCoords;
in vec4		v_Color;

uniform sampler2D	u_texture;
uniform vec4		u_color;

void main()
{
	vec4 texColor = u_color * texture(u_texture, v_TexCoords);
	v_FragColor = texColor;
	//v_FragColor = vec4(1.0);
}