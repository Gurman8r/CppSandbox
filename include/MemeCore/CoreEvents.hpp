#ifndef _CORE_EVENTS_HPP_
#define _CORE_EVENTS_HPP_

#include <MemeCore/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API CoreEvent : public Event
	{
		enum : int32_t
		{
			MIN_CORE_EVENT = Event::EV_CORE + 1,
			MAX_CORE_EVENT
		};

		CoreEvent(int32_t id)
			: Event(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_CORE_EVENTS_HPP_
