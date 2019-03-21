#ifndef _ML_CORE_EVENTS_HPP_
#define _ML_CORE_EVENTS_HPP_

#include <MemeCore/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API CoreEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_CORE_EVENT = IEvent::EV_CORE + 1,
			EV_RequestExit,
			EV_FileSystem,
			MAX_CORE_EVENT
		};

		CoreEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API RequestExitEvent : public CoreEvent
	{
		RequestExitEvent()
			: CoreEvent(EV_RequestExit)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name() << "] ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FileSystemEvent : public CoreEvent
	{
		FileSystemEvent()
			: CoreEvent(EV_FileSystem)
		{
		}

		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name() << "] ";
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CORE_EVENTS_HPP_