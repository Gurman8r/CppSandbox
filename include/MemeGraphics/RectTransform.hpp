#ifndef _ML_RECT_TRANSFORM_HPP_
#define _ML_RECT_TRANSFORM_HPP_

#include <MemeCore/Transform.hpp>
#include <MemeCore/Rect.hpp>
#include <MemeGraphics/Export.hpp>

namespace ml
{
	class ML_GRAPHICS_API RectTransform
		: public Transform
	{
	public:
		RectTransform();
		RectTransform(const vec2f & position, const vec2f & scale, const float rotation);
		RectTransform(const RectTransform & copy);
		~RectTransform();

		void update();

	private:
		vec2f m_origin;
		vec2f m_position;
		float m_rotation;
		vec2f m_scale;
	};
}

#endif // !_ML_RECT_TRANSFORM_HPP_