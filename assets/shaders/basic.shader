// basic.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#include <common/Vert.MVP.shader>

void main()
{
	gl_Position = ml_MVP_Position();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include <common/Frag.Draw.shader>

void main()
{
	gl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */