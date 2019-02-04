#ifndef _GIZMOS_HPP_
#define _GIZMOS_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/ISingleton.hpp>
#include <MemeCore/Vector3.hpp>

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

#endif // !_GIZMOS_HPP_
