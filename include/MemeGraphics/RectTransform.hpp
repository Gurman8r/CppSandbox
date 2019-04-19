#ifndef _ML_RECT_TRANSFORM_HPP_
#define _ML_RECT_TRANSFORM_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RectTransform final
		: public ITrackable
	{
	public:
		RectTransform();
		RectTransform(const vec2f & position, const vec2f & scale, const float rotation, const vec2f & origin);
		RectTransform(const RectTransform & copy);
		~RectTransform();

	public:
		void update() const;

	public:
		RectTransform & setOrigin	(const vec2f &	value);
		RectTransform & setPosition	(const vec2f &	value);
		RectTransform & setRotation	(const float	value);
		RectTransform & setScale	(const vec2f &	value);

	public:
		inline const vec2f & origin()	const { return m_origin;	}
		inline const vec2f & position() const { return m_position;	}
		inline const float & rotation() const { return m_rotation;	}
		inline const vec2f & scale()	const { return m_scale;		}

	public:
		inline const mat4f & matrix() const
		{
			update();
			return m_matrix;
		}

	private:
		vec2f m_origin;
		vec2f m_position;
		float m_rotation;
		vec2f m_scale;

		mutable mat4f	m_matrix;
		mutable bool	m_changed;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RECT_TRANSFORM_HPP_