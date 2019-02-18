#ifndef _DEMO_EVENTS_HPP_
#define _DEMO_EVENTS_HPP_

#include <MemeCore/Event.hpp>
#include <MemeCore/Duration.hpp>
#include <MemeGraphics/RenderWindow.hpp>

namespace DEMO 
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct DemoEvent : public ml::Event
	{
		enum : int32_t
		{
			MIN_DEMO_EVENT = ml::Event::EV_CUSTOM_0 + 1,

			EV_Enter,
			EV_Load,
			EV_Start,
			EV_Update,
			EV_Draw,
			EV_Gui,
			EV_Exit,

			MAX_DEMO_EVENT
		};

		DemoEvent(int32_t id)
			: ml::Event(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct EnterEvent final : public DemoEvent
	{
		int32_t argc;
		char ** argv;

		EnterEvent(int32_t argc, char ** argv)
			: DemoEvent(EV_Enter)
			, argc(argc)
			, argv(argv)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct LoadEvent final : public DemoEvent
	{
		LoadEvent()
			: DemoEvent(EV_Load)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct StartEvent final : public DemoEvent
	{
		StartEvent()
			: DemoEvent(EV_Start)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct UpdateEvent final : public DemoEvent
	{
		const ml::Duration & elapsed;

		UpdateEvent(const ml::Duration & elapsed)
			: DemoEvent(EV_Update)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct DrawEvent final : public DemoEvent
	{
		const ml::Duration & elapsed;

		DrawEvent(const ml::Duration & elapsed)
			: DemoEvent(EV_Draw)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct GuiEvent final : public DemoEvent
	{
		const ml::Duration & elapsed;

		GuiEvent(const ml::Duration & elapsed)
			: DemoEvent(EV_Gui)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ExitEvent final : public DemoEvent
	{
		ExitEvent()
			: DemoEvent(EV_Exit)
		{
		}
	};
}

#endif // !_DEMO_EVENTS_HPP_
