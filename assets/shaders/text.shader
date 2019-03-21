// text.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader vertex
#include "../../../assets/shaders/common/Vert.MVP.shader"

void main()
{
	ml_Vert_Update();

	gl_Position = Vert.proj * vec4(Out.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#shader fragment
#include "../../../assets/shaders/common/Frag.Draw.shader"

void main()
{
	gl_Color = ml_Frag_MainCol() * vec4(1, 1, 1, ml_Frag_MainPixel().r);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */