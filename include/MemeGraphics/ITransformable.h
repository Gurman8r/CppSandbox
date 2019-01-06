#ifndef _ITRANSFORMABLE_H_
#define _ITRANSFORMABLE_H_

#include <MemeGraphics/Transform.h>

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

	protected:
		Transform m_transform;
	};
}

#endif // !_ITRANSFORMABLE_H_
