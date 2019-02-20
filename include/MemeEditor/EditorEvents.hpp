#ifndef _EDITOR_EVENTS_HPP_
#define _EDITOR_EVENTS_HPP_

#include <MemeCore/Event.hpp>
#include <MemeEditor/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorEvent : public Event
	{
		enum : int32_t
		{
			MIN_EDITOR_EVENT = Event::EV_EDITOR + 1,
			MAX_EDITOR_EVENT
		};

		EditorEvent(int32_t id)
			: Event(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_EDITOR_EVENTS_HPP_
