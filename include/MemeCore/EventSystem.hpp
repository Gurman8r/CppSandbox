#ifndef _EVENT_SYSTEM_HPP_
#define _EVENT_SYSTEM_HPP_

#include <MemeCore/IEventListener.hpp>
#include <MemeCore/ISingleton.hpp>

#define ML_EventSystem ml::EventSystem::getInstance()

namespace ml
{
	class ML_CORE_API EventSystem final
		: public ISingleton<EventSystem>
		, public ITrackable
	{
		friend class ISingleton<EventSystem>;

	public:
		void addListener(const int32_t & type, IEventListener * listener);
		void fireEvent(const Event & ev);
		void removeListener(const int32_t & type, IEventListener * listener);
		void removeListenerFromAllEvents(IEventListener * listener);

	private:
		EventSystem() {}
		~EventSystem() {}

		void dispatchAllEvents(const Event * ev);

		std::multimap<int32_t, IEventListener *> m_listeners;
	};
}

#endif // !_EVENT_SYSTEM_HPP_
