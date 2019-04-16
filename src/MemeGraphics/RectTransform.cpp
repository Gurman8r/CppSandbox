#include <MemeGraphics/RectTransform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RectTransform::RectTransform()
	{
	}

	RectTransform::RectTransform(const vec2f & position, const vec2f & scale, const float rotation)
	{
	}

	RectTransform::RectTransform(const RectTransform & copy)
	{
	}

	RectTransform::~RectTransform()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void RectTransform::update()
	{
		float angle		= m_rotation * Maths::Deg2Rad;
		float cosine	= cosf(angle);
		float sine		= sinf(angle);
        float sxc		= m_scale[0] * cosine;
        float syc		= m_scale[1] * cosine;
        float sxs		= m_scale[0] * sine;
        float sys		= m_scale[1] * sine;
        float tx		= -m_origin[0] * sxc - m_origin[1] * sys + m_position[0];
        float ty		=  m_origin[0] * sxs - m_origin[1] * syc + m_position[1];

		float m00 =  sxc; float m01 = sys; float m02 = tx;
		float m03 = -sxs; float m04 = syc; float m05 = ty;
		float m10 =  0.f; float m11 = 0.f; float m12 = 0.f;

		matrix() = {
			m00, m01, 0.f, m02,
			m03, m04, 0.f, m05,
			0.f, 0.f, 1.f, 0.f,
			m10, m11, 0.f, m12,
		};
	}

	/* * * * * * * * * * * * * * * * * * * * */
}