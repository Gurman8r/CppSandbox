#ifndef _EVENT_HPP_
#define _EVENT_HPP_

#include <MemeCore/ITrackable.hpp>

#define ML_MAX_SYSTEM_EVENTS 32
#define ML_MAX_CUSTOM_EVENTS 256

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
			
			MAX_SYSTEM	= MIN_SYSTEM + (SIZE_SYSTEM * 6),


			// Custom
			/* * * * * * * * * * * * * * */
			SIZE_CUSTOM = ML_MAX_CUSTOM_EVENTS,
			MIN_CUSTOM	= MAX_SYSTEM + 1,

			EV_CUSTOM_0 = MIN_CUSTOM + (SIZE_CUSTOM * 0),
			EV_CUSTOM_1 = MIN_CUSTOM + (SIZE_CUSTOM * 1),
			EV_CUSTOM_2 = MIN_CUSTOM + (SIZE_CUSTOM * 2),
			EV_CUSTOM_3 = MIN_CUSTOM + (SIZE_CUSTOM * 3),
			EV_CUSTOM_4 = MIN_CUSTOM + (SIZE_CUSTOM * 4),
			EV_CUSTOM_5 = MIN_CUSTOM + (SIZE_CUSTOM * 5),
			EV_CUSTOM_6 = MIN_CUSTOM + (SIZE_CUSTOM * 6),
			EV_CUSTOM_7 = MIN_CUSTOM + (SIZE_CUSTOM * 7),
			EV_CUSTOM_8 = MIN_CUSTOM + (SIZE_CUSTOM * 8),
			EV_CUSTOM_9 = MIN_CUSTOM + (SIZE_CUSTOM * 9),
			
			MAX_CUSTOM = MIN_CUSTOM + (SIZE_CUSTOM * 10),

			/* * * * * * * * * * * * * * */

			MAX_EVENT_ID
		};

	public:
		Event(int32_t id)
			: m_eventID(id)
		{
		}
		virtual ~Event() {}

		inline const int32_t & eventID() const 
		{ 
			return m_eventID;
		}

		inline operator bool() const 
		{ 
			return eventID(); 
		}

	public:
		template <class T>
		inline const T * As() const
		{
			return dynamic_cast<const T *>(this);
		}

		template <class T>
		inline T * As()
		{
			return dynamic_cast<T *>(this);
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

#endif // !_EVENT_HPP_
