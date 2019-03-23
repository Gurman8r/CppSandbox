#ifndef _ML_SCRIPT_EVENTS_HPP_
#define _ML_SCRIPT_EVENTS_HPP_

#include <MemeCore/IEvent.hpp>
#include <MemeScript/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API ScriptEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_SCRIPT_EVENT = IEvent::EV_SCRIPT + 1,
			MAX_SCRIPT_EVENT
		};

		ScriptEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCRIPT_EVENTS_HPP_