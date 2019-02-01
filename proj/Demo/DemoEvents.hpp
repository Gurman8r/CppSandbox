#ifndef _DEMO_EVENTS_HPP_
#define _DEMO_EVENTS_HPP_

#include <MemeCore/Event.hpp>
#include <MemeCore/Duration.hpp>
#include <MemeCore/InputState.hpp>
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
		bool log;

		LoadEvent(bool log)
			: DemoEvent(EV_Load)
			, log(log)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct StartEvent final : public DemoEvent
	{
		ml::RenderWindow & window;

		StartEvent(ml::RenderWindow & window)
			: DemoEvent(EV_Start)
			, window(window)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct UpdateEvent final : public DemoEvent
	{
		ml::RenderWindow &	window;
		ml::Duration		elapsed;
		ml::NativeInput		input;

		UpdateEvent(
			ml::RenderWindow & window, 
			const ml::Duration & elapsed,
			const ml::NativeInput & input)
			: DemoEvent(EV_Update)
			, window(window)
			, elapsed(elapsed)
			, input(input)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct DrawEvent final : public DemoEvent
	{
		ml::RenderWindow & window;
		ml::Duration elapsed;

		DrawEvent(ml::RenderWindow & window, const ml::Duration & elapsed)
			: DemoEvent(EV_Draw)
			, window(window)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ExitEvent final : public DemoEvent
	{
		int32_t exitCode;

		ExitEvent(int32_t exitCode)
			: DemoEvent(EV_Exit)
			, exitCode(exitCode)
		{
		}
	};
}

#endif // !_DEMO_EVENTS_HPP_
