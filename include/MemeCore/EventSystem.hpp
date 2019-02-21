#ifndef _EVENT_SYSTEM_HPP_
#define _EVENT_SYSTEM_HPP_

#include <MemeCore/IEventListener.hpp>
#include <MemeCore/ISingleton.hpp>

#define ML_EventSystem ml::EventSystem::getInstance()

namespace ml
{
	class ML_CORE_API EventSystem final
		: public ITrackable
		, public ISingleton<EventSystem>
	{
		friend class ISingleton<EventSystem>;

	public:
		using multimap_type	= MultiMap<int32_t, IEventListener *>;
		using iterator		= typename multimap_type::iterator;

	public:
		void addListener(const int32_t & type, IEventListener * listener);
		void fireEvent(const Event & ev);
		void removeListener(const int32_t & type, IEventListener * listener);
		void removeListenerFromAllEvents(IEventListener * listener);

	private:
		EventSystem() {}
		~EventSystem() {}

		void dispatchAllEvents(const Event * ev);

		multimap_type m_listeners;
	};
}

#endif // !_EVENT_SYSTEM_HPP_
