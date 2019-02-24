#ifndef _AUDIO_EVENTS_HPP_
#define _AUDIO_EVENTS_HPP_

#include <MemeAudio/Export.hpp>
#include <MemeCore/IEvent.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_AUDIO_API AudioEvent : public IEvent
	{
		enum : int32_t
		{
			MIN_AUDIO_EVENT = IEvent::EV_AUDIO + 1,
			MAX_AUDIO_EVENT
		};

		AudioEvent(int32_t id)
			: IEvent(id)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_AUDIO_EVENTS_HPP_
