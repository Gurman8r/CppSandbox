#include <MemeCore/EventSystem.h>

namespace ml
{
	void EventSystem::addListener(const int & type, EventListener * listener)
	{
		m_listeners.insert({ type, listener });
	}
	
	void EventSystem::fireEvent(const EventBase & ev)
	{
		dispatchAllEvents(&ev);
	}
	
	void EventSystem::removeListener(const int & type, EventListener * listener)
	{
		std::pair<
			std::multimap<int, EventListener*>::iterator,
			std::multimap<int, EventListener*>::iterator> ret;

		ret = m_listeners.equal_range(type);

		std::multimap<int, EventListener*>::iterator iter;

		for (iter = ret.first; iter != ret.second; ++iter)
		{
			if (iter->second == listener)
			{
				m_listeners.erase(iter);

				break; // to prevent using invalidated iterator
			}
		}
	}
	
	void EventSystem::removeListenerFromAllEvents(EventListener * listener)
	{
		std::multimap<int, EventListener*>::iterator iter;

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

	void EventSystem::dispatchAllEvents(const EventBase * ev)
	{
		std::pair<
			std::multimap<int, EventListener*>::iterator,
			std::multimap<int, EventListener*>::iterator> ret;

		ret = m_listeners.equal_range(ev->id());

		std::multimap<int, EventListener*>::iterator it;

		for (it = ret.first; it != ret.second; ++it)
		{
			it->second->handleEvent(ev);
		}
	}
}