#ifndef _ML_EDITOR_EVENTS_HPP_
#define _ML_EDITOR_EVENTS_HPP_

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

			EV_MainMenuBar,
			EV_DockBuilder,
			EV_Inspector,

			MAX_EDITOR_EVENT
		};

		EditorEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API MainMenuBarEvent : public EditorEvent
	{
		MainMenuBarEvent()
			: EditorEvent(EV_MainMenuBar)
		{
		}

		void serialize(std::ostream & out) const override
		{
			out << get_type().name();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API DockBuilderEvent : public EditorEvent
	{
		int32_t root;

		DockBuilderEvent(int32_t root)
			: EditorEvent(EV_DockBuilder)
			, root(root)
		{
		}

		void serialize(std::ostream & out) const override
		{
			out << get_type().name() << ' ' << root;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API InspectorEvent : public EditorEvent
	{
		InspectorEvent()
			: EditorEvent(EV_Inspector)
		{
		}

		void serialize(std::ostream & out) const override
		{
			out << get_type().name();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_EVENTS_HPP_