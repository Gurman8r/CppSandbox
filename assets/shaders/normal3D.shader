// normal3D.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#include "../../../assets/shaders/common/Vert.MVP.shader"

void main()
{
	gl_Position = ml_Vert_Position();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include "../../../assets/shaders/common/Frag.Draw.shader"

void main()
{
	gl_Color = In.Normal * ml_Frag_MainCol() * ml_Frag_MainPixel();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */