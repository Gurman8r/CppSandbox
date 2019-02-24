#ifndef _GRAPHICS_EVENTS_HPP_
#define _GRAPHICS_EVENTS_HPP_

#include <MemeCore/IEvent.hpp>
#include <MemeGraphics/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API GraphicsEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_GRAPHICS_EVENT = IEvent::EV_GRAPHICS + 1,
			MAX_GRAPHICS_EVENT
		};

		GraphicsEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_GRAPHICS_EVENTS_HPP_
