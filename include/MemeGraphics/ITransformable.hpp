#ifndef _ML_ITRANSFORMABLE_HPP_
#define _ML_ITRANSFORMABLE_HPP_

#include <MemeCore/Transform.hpp>

namespace ml
{
	class ML_GRAPHICS_API ITransformable
	{
	public:
		virtual ~ITransformable() {}

	public:
		inline Transform & transform()
		{
			return m_transform;
		}

		inline const Transform & transform() const 
		{
			return m_transform;
		}
		
		inline Transform & transform(const Transform & value)
		{
			return (m_transform = value);
		}

	private:
		Transform m_transform;
	};
}

#endif // !_ML_I_TRANSFORMABLE_HPP_