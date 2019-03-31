#ifndef _ML_ENGINE_EVENTS_HPP_
#define _ML_ENGINE_EVENTS_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/IEvent.hpp>
#include <MemeCore/Time.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT EngineEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_ENGINE_EVENT = IEvent::EV_ENGINE + 1,

			EV_Enter,
			EV_Load,
			EV_Start,
			EV_FixedUpdate,
			EV_Update,
			EV_Draw,
			EV_Unload,
			EV_Exit,

			MAX_ENGINE_EVENT
		};

		EngineEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT EnterEvent final : public EngineEvent
	{
		int32_t argc;
		char ** argv;

		EnterEvent(int32_t argc, char ** argv)
			: EngineEvent(EV_Enter)
			, argc(argc)
			, argv(argv)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT LoadEvent final : public EngineEvent
	{
		LoadEvent()
			: EngineEvent(EV_Load)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT StartEvent final : public EngineEvent
	{
		StartEvent()
			: EngineEvent(EV_Start)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT FixedUpdateEvent final : public EngineEvent
	{
		const Duration & elapsed;

		FixedUpdateEvent(const Duration & elapsed)
			: EngineEvent(EV_FixedUpdate)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT UpdateEvent final : public EngineEvent
	{
		const Duration & elapsed;

		UpdateEvent(const Duration & elapsed)
			: EngineEvent(EV_Update)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT DrawEvent final : public EngineEvent
	{
		const Duration & elapsed;

		DrawEvent(const Duration & elapsed)
			: EngineEvent(EV_Draw)
			, elapsed(elapsed)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT UnloadEvent final : public EngineEvent
	{
		UnloadEvent()
			: EngineEvent(EV_Unload)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_API_EXPORT ExitEvent final : public EngineEvent
	{
		ExitEvent()
			: EngineEvent(EV_Exit)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_EVENTS_HPP_