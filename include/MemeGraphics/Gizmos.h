#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/ISingleton.h>
#include <MemeCore/Vector3.h>

#define ML_GIZMOS ml::Gizmos::getInstance()

namespace ml
{
	class ML_GRAPHICS_API Gizmos final
		: public ITrackable
		, public ISingleton<Gizmos>
	{
		friend class ISingleton<Gizmos>;

	public:
		void drawAxisImmediate(const mat4f & mvp, const vec3f & pos, float size);

	private:
		Gizmos() {}
		~Gizmos() {}

	};
}

#endif // !_GIZMOS_H_
