#ifndef _PHYSICS_EVENTS_HPP_
#define _PHYSICS_EVENTS_HPP_

#include <MemeCore/Event.hpp>
#include <MemePhysics/Export.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PHYSICS_API PhysicsEvent : public Event
	{
		enum : int32_t
		{
			MIN_PHYSICS_EVENT = Event::EV_PHYSICS + 1,
			MAX_PHYSICS_EVENT
		};

		PhysicsEvent(int32_t id)
			: Event(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PHYSICS_EVENTS_HPP_
