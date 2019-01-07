#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/ISingleton.h>

#define ML_GIZMOS ml::Gizmos::getInstance()

namespace ml
{
	class ML_GRAPHICS_API Gizmos final
		: public ITrackable
		, public ISingleton<Gizmos>
	{
		friend class ISingleton<Gizmos>;

	public:

	private:
		Gizmos() {}
		~Gizmos() {}

	};
}

#endif // !_GIZMOS_H_
