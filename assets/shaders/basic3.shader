// basic.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Vert.MVP.shader"
#shader vertex

void main()
{
	gl_Position = ml_MVP();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Frag.Tex.shader"
#shader fragment

void main()
{
	gl_Color = (Frag.mainCol * texture(Frag.mainTex, In.Texcoord));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */