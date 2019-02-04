#ifndef _I_EVENT_LISTENER_HPP_
#define _I_EVENT_LISTENER_HPP_

#include <MemeCore/Event.hpp>

namespace ml
{
	class ML_CORE_API IEventListener
	{
	public:
		virtual void onEvent(const Event * ev) = 0;
	};
}

#endif // !_I_EVENT_LISTENER_HPP_
