#ifndef _RECT_H_
#define _RECT_H_

#include <MemeCore/Export.h>
#include <MemeCore/Vector2.h>
#include <MemeCore/Vector4.h>

namespace ml
{
	class ML_CORE_API Rect final
		: public ITrackable
	{
	public:
		Rect() {}
		Rect(float x, float y, float w, float h) {}
		Rect(const vec2f & pos, const vec2f & size) {}
		Rect(const vec4f & bounds) {}
		Rect(const Rect & copy) {}
		~Rect() {}

	private:

	};
}

#endif // !_RECT_H_
