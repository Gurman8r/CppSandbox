#ifndef _ML_ITRANSFORMABLE_HPP_
#define _ML_ITRANSFORMABLE_HPP_

#include <MemeGraphics/Transform.hpp>

namespace ml
{
	class ML_GRAPHICS_API ITransformable
	{
	public:
		virtual ~ITransformable() {}

		inline const Transform & transform() const 
		{
			return m_transform;
		}
		
		inline Transform & transform() 
		{ 
			return m_transform;
		}
		
		inline ITransformable &	transform(const Transform & value)
		{
			m_transform = value;
			return (*this);
		}

	private:
		Transform m_transform;
	};
}

#endif // !_ML_I_TRANSFORMABLE_HPP_