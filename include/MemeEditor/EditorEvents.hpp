#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

#include <MemeCore/IEvent.hpp>
#include <MemeEditor/Export.hpp>
#include <MemeCore/Time.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_EDITOR_EVENT = IEvent::EV_EDITOR + 1,

			EV_Gui,
			EV_Inspector,

			MAX_EDITOR_EVENT
		};

		EditorEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API GuiEvent final : public EditorEvent
	{
		const Duration & elapsed;

		GuiEvent(const Duration & elapsed)
			: EditorEvent(EV_Gui)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API InspectorEvent : public EditorEvent
	{
		InspectorEvent()
			: EditorEvent(EV_Inspector)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_