#version 330 core

// varyings
// * * * * * * * * 
out vec4	v_FragColor;
in vec2		v_TexCoords;
in vec4		v_Color;

// uniforms
// * * * * * * * *
uniform int			u_solid;
uniform sampler2D	u_texture;
uniform vec4		u_color;
uniform vec2		u_texOffset;

// main
// * * * * * * * *
void main()
{    
	if(u_solid == 1)
	{
		v_FragColor = u_color * v_Color;
	}
	else
	{
		vec2 tc = v_TexCoords + u_texOffset;

		v_FragColor = texture(u_texture, tc) * u_color * v_Color;
	}
}  