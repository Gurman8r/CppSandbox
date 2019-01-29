#ifndef _EVENT_H_
#define _EVENT_H_

#include <MemeCore/ITrackable.h>

#ifndef ML_MAX_SYSTEM_EVENTS
#define ML_MAX_SYSTEM_EVENTS 32
#endif

#ifndef ML_MAX_CUSTOM_EVENTS
#define ML_MAX_CUSTOM_EVENTS 256
#endif

namespace ml
{
	class ML_CORE_API Event
		: public ITrackable
	{
	public:
		enum : int32_t
		{
			MIN_EVENT_ID	= -1,

			// System
			/* * * * * * * * * * * * * * */
			SIZE_SYSTEM	= ML_MAX_SYSTEM_EVENTS,
			MIN_SYSTEM	= 0,

			EV_AUDIO	= MIN_SYSTEM + (SIZE_SYSTEM * 1),
			EV_CORE		= MIN_SYSTEM + (SIZE_SYSTEM * 2),
			EV_GRAPHICS = MIN_SYSTEM + (SIZE_SYSTEM * 3),
			EV_NETWORK	= MIN_SYSTEM + (SIZE_SYSTEM * 4),
			EV_WINDOW	= MIN_SYSTEM + (SIZE_SYSTEM * 5),
			
			MAX_SYSTEM	= EV_WINDOW + (SIZE_SYSTEM + 1),

			// Custom
			/* * * * * * * * * * * * * * */
			SIZE_CUSTOM = ML_MAX_CUSTOM_EVENTS,
			MIN_CUSTOM	= MAX_SYSTEM + 1,

			EV_CUSTOM_0 = MIN_CUSTOM + (SIZE_CUSTOM * 0),
			EV_CUSTOM_1 = MIN_CUSTOM + (SIZE_CUSTOM * 1),
			EV_CUSTOM_2 = MIN_CUSTOM + (SIZE_CUSTOM * 2),
			EV_CUSTOM_3 = MIN_CUSTOM + (SIZE_CUSTOM * 3),
			EV_CUSTOM_4 = MIN_CUSTOM + (SIZE_CUSTOM * 4),
			
			MAX_CUSTOM = EV_CUSTOM_4 + (SIZE_CUSTOM + 1),

			/* * * * * * * * * * * * * * */

			MAX_EVENT_ID
		};

	public:
		Event(int32_t id)
			: m_eventID(id)
		{
		}
		virtual ~Event() {}

		inline const int32_t & eventID() const { return m_eventID; }

		inline operator bool() const { return eventID(); }

	public:
		template <class T>
		inline const T * As() const
		{
			return dynamic_cast<const T*>(this);
		}

		template <class T>
		inline T * As()
		{
			return dynamic_cast<T*>(this);
		}

		template <class T>
		inline const bool Is() const
		{
			return bool(As<T>());
		}

	private:
		int32_t m_eventID;

	};
}

#endif // !_EVENT_H_
