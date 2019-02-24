#ifndef _EVENT_SERIALIZER_HPP_
#define _EVENT_SERIALIZER_HPP_

#include <MemeAudio/AudioEvents.hpp>
#include <MemeCore/CoreEvents.hpp>
#include <MemeEditor/EditorEvents.hpp>
#include <MemeGraphics/GraphicsEvents.hpp>
#include <MemeNet/NetworkEvents.hpp>
#include <MemePhysics/PhysicsEvents.hpp>
#include <MemeScript/ScriptEvents.hpp>
#include <MemeWindow/WindowEvents.hpp>

#define ML_EventSerializer ml::EventSerializer::getInstance()

namespace ml
{
	class ML_EDITOR_API EventSerializer
		: public ITrackable
		, public ISingleton<EventSerializer>
	{
		friend class ISingleton<EventSerializer>;

	public:
		template <class E>
		using NewEventFun = E * (*)(std::istream &);

	public:
		IEvent * deserializeEvent(std::istream & in) const;

	private:
		EventSerializer();
		~EventSerializer();
	};
}

#endif // !_EVENT_SERIALIZER_HPP_
