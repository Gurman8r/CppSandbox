#include <MemeCore/EventSystem.hpp>

namespace ml
{
	void EventSystem::addListener(const int32_t & type, IEventListener * listener)
	{
		m_listeners.insert({ type, listener });
	}
	
	void EventSystem::fireEvent(const Event & ev)
	{
		dispatchAllEvents(&ev);
	}
	
	void EventSystem::removeListener(const int32_t & type, IEventListener * listener)
	{
		std::pair<
			std::multimap<int32_t, IEventListener *>::iterator,
			std::multimap<int32_t, IEventListener *>::iterator> ret;

		ret = m_listeners.equal_range(type);

		std::multimap<int32_t, IEventListener *>::iterator iter;

		for (iter = ret.first; iter != ret.second; ++iter)
		{
			if (iter->second == listener)
			{
				m_listeners.erase(iter);

				break; // to prevent using invalidated iterator
			}
		}
	}
	
	void EventSystem::removeListenerFromAllEvents(IEventListener * listener)
	{
		std::multimap<int32_t, IEventListener *>::iterator iter;

		bool allTheWayThrough = false;

		while (!allTheWayThrough)
		{
			allTheWayThrough = true;

			for (iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
			{
				if (iter->second == listener)
				{
					m_listeners.erase(iter);

					allTheWayThrough = false; // didn't make it the whole way through

					break; // to prevent using invalidated iterator
				}
			}
		}
	}

	void EventSystem::dispatchAllEvents(const Event * ev)
	{
		std::pair<
			std::multimap<int32_t, IEventListener *>::iterator,
			std::multimap<int32_t, IEventListener *>::iterator> ret;

		ret = m_listeners.equal_range(ev->eventID());

		std::multimap<int32_t, IEventListener *>::iterator it;

		for (it = ret.first; it != ret.second; ++it)
		{
			it->second->onEvent(ev);
		}
	}
}