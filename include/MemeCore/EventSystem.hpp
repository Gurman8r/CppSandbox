#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include <MemeCore/EventListener.hpp>
#include <MemeCore/ISingleton.hpp>
#include <map>

#define ML_EventSystem ml::EventSystem::getInstance()

namespace ml
{
	class ML_CORE_API EventSystem final
		: public ISingleton<EventSystem>
		, public ITrackable
	{
		friend class ISingleton<EventSystem>;

	public:
		void addListener(const int32_t & type, EventListener * listener);
		void fireEvent(const Event & ev);
		void removeListener(const int32_t & type, EventListener * listener);
		void removeListenerFromAllEvents(EventListener * listener);

	private:
		EventSystem() {}
		~EventSystem() {}

		void dispatchAllEvents(const Event * ev);

		std::multimap<int32_t, EventListener*> m_listeners;
	};
}

#endif // !_EVENT_SYSTEM_H_
