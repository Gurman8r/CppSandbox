#ifndef _EVENT_LISTENER_H_
#define _EVENT_LISTENER_H_

#include <MemeCore/EventBase.h>

namespace ml
{
	class ML_CORE_API EventListener
	{
	public:
		virtual void handleEvent(const EventBase * ev) = 0;

	private:

	};
}

#endif // !_EVENT_LISTENER_H_
