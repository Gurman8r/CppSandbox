#ifndef _GRAPHICS_EVENTS_HPP_
#define _GRAPHICS_EVENTS_HPP_

#include <MemeCore/Event.hpp>
#include <MemeGraphics/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API GraphicsEvent : public Event
	{
		enum : int32_t
		{
			MIN_GRAPHICS_EVENT = Event::EV_GRAPHICS + 1,
			MAX_GRAPHICS_EVENT
		};

		GraphicsEvent(int32_t id)
			: Event(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_GRAPHICS_EVENTS_HPP_
