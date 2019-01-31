#include <MemeGraphics/Gizmos.hpp>
#include <MemeGraphics/OpenGL.hpp>

namespace ml
{
	void Gizmos::drawAxisImmediate(const mat4f & mvp, const vec3f & pos, float size)
	{
		OpenGL::pushMatrix();
		OpenGL::loadMatrix(mvp.ptr());
		OpenGL::begin(GL::Lines);
		{
			OpenGL::color3f(1.0f, 0.0f, 0.0f);
			OpenGL::vertex3f(pos[0] + size, pos[1], pos[2]);
			OpenGL::vertex3f(pos[0], pos[1], pos[2]);

			OpenGL::color3f(0.0f, 1.0f, 0.0f);
			OpenGL::vertex3f(pos[0], pos[1] + size, pos[2]);
			OpenGL::vertex3f(pos[0], pos[1], pos[2]);

			OpenGL::color3f(0.0f, 0.0f, 1.0f);
			OpenGL::vertex3f(pos[0], pos[1], pos[2] + size);
			OpenGL::vertex3f(pos[0], pos[1], pos[2]);
		}
		OpenGL::end();
		OpenGL::popMatrix();
	}
}