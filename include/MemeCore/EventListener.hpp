#ifndef _EVENT_LISTENER_HPP_
#define _EVENT_LISTENER_HPP_

#include <MemeCore/Event.hpp>

namespace ml
{
	class ML_CORE_API EventListener
	{
	public:
		virtual void onEvent(const Event * ev) = 0;

	private:

	};
}

#endif // !_EVENT_LISTENER_HPP_
