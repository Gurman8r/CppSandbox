#ifndef _AUDIO_EVENTS_HPP_
#define _AUDIO_EVENTS_HPP_

#include <MemeAudio/Export.hpp>
#include <MemeCore/Event.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_AUDIO_API AudioEvent : public Event
	{
		enum : int32_t
		{
			MIN_AUDIO_EVENT = Event::EV_AUDIO + 1,
			MAX_AUDIO_EVENT
		};

		AudioEvent(int32_t id)
			: Event(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_AUDIO_EVENTS_HPP_
