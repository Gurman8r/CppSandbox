#ifndef _DEMO_EVENTS_HPP_
#define _DEMO_EVENTS_HPP_

#include <MemeCore/IEvent.hpp>
#include <MemeCore/Duration.hpp>
#include <MemeGraphics/RenderWindow.hpp>
#include <MemeScript/Args.hpp>

namespace DEMO 
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct DemoEvent : public ml::IEvent
	{
		enum : int32_t
		{
			MIN_DEMO_EVENT = ml::IEvent::EV_CUSTOM_0 + 1,

			EV_Enter,
			EV_Load,
			EV_Start,
			EV_FixedUpdate,
			EV_Update,
			EV_Draw,
			EV_Gui,
			EV_Unload,
			EV_Exit,

			MAX_DEMO_EVENT
		};

		DemoEvent(int32_t id)
			: ml::IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct EnterEvent final : public DemoEvent
	{
		ml::Args args;

		EnterEvent(const ml::Args & args)
			: DemoEvent(EV_Enter)
			, args(args)
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

	struct FixedUpdateEvent final : public DemoEvent
	{
		const ml::Duration & elapsed;

		FixedUpdateEvent(const ml::Duration & elapsed)
			: DemoEvent(EV_FixedUpdate)
			, elapsed(elapsed)
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

	struct UnloadEvent final : public DemoEvent
	{
		UnloadEvent()
			: DemoEvent(EV_Unload)
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

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_DEMO_EVENTS_HPP_
