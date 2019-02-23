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
			EV_RequestExit,
			MAX_CORE_EVENT
		};

		CoreEvent(int32_t id)
			: Event(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API RequestExitEvent : public CoreEvent
	{
		RequestExitEvent()
			: CoreEvent(EV_RequestExit)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name() << "] ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_CORE_EVENTS_HPP_
