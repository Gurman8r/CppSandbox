#ifndef _EDITOR_EVENTS_HPP_
#define _EDITOR_EVENTS_HPP_

#include <MemeCore/IEvent.hpp>
#include <MemeEditor/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_EDITOR_EVENT = IEvent::EV_EDITOR + 1,
			MAX_EDITOR_EVENT
		};

		EditorEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_EDITOR_EVENTS_HPP_
