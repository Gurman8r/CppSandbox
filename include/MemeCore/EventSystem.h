#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include <MemeCore/EventListener.h>
#include <MemeCore/ISingleton.h>
#include <map>

#define ML_EventSystem ml::EventSystem::getInstance()

namespace ml
{
	class EventSystem final
		: public ISingleton<EventSystem>
	{
		friend class ISingleton<EventSystem>;

	public:
		void addListener(const int32_t & type, EventListener * listener);
		void fireEvent(const EventBase & ev);
		void removeListener(const int32_t & type, EventListener * listener);
		void removeListenerFromAllEvents(EventListener * listener);

	private:
		EventSystem() {}
		~EventSystem() {}

		void dispatchAllEvents(const EventBase * ev);

		std::multimap<int32_t, EventListener*> m_listeners;
	};
}

#endif // !_EVENT_SYSTEM_H_
