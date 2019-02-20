#ifndef _SCRIPT_EVENTS_HPP_
#define _SCRIPT_EVENTS_HPP_

#include <MemeCore/Event.hpp>
#include <MemeScript/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API ScriptEvent : public Event
	{
		enum : int32_t
		{
			MIN_SCRIPT_EVENT = Event::EV_SCRIPT + 1,
			MAX_SCRIPT_EVENT
		};

		ScriptEvent(int32_t id)
			: Event(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SCRIPT_EVENTS_HPP_
