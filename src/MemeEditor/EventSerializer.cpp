#include <MemeEditor/EventSerializer.hpp>

namespace ml
{
	template <class E>
	inline static E * loadEvent(std::istream & in)
	{
		return NULL;
	}
}

namespace ml
{
	EventSerializer::EventSerializer()
	{
		const std::type_info & type = typeid(RequestExitEvent);
		NewEventFun<RequestExitEvent> fun = loadEvent<RequestExitEvent>;
	}

	EventSerializer::~EventSerializer()
	{
	}

	IEvent * EventSerializer::deserializeEvent(std::istream & in) const
	{
		if (in.good())
		{
			String first; in >> first;

			if ((first == "event") && in.good())
			{
				String type; in >> type;

				if (type.empty())
				{
					return NULL;
				}
				else if (type == "ml::RequestExitEvent") { return new RequestExitEvent(); }
			}
		}
		return NULL;
	}
}