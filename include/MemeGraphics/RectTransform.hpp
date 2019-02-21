#ifndef _RECT_TRANSFORM_HPP_
#define _RECT_TRANSFORM_HPP_

#include <MemeGraphics/Transform.hpp>
#include <MemeCore/Rect.hpp>

namespace ml
{
	class ML_GRAPHICS_API RectTransform
		: public ITrackable
	{
	public:
		RectTransform();
		~RectTransform();

	private:
		FloatRect m_bounds;
	};
}

#endif // !_RECT_TRANSFORM_HPP_
