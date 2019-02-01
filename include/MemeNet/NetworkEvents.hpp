#ifndef _NETWORK_EVENTS_HPP_
#define _NETWORK_EVENTS_HPP_

#include <MemeNet/Export.hpp>
#include <MemeCore/Event.hpp>

namespace ml
{
	// base_type
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API NetworkEvent : public Event
	{
		enum : int32_t
		{
			MIN_NETWORK_EVENT = Event::EV_NETWORK + 1,

			EV_ServerRecievePacket,
			EV_ClientRecievePacket,

			MAX_NETWORK_EVENT
		};
		
		NetworkEvent(int32_t id)
			: Event(id)
		{
		}
	};


	// Server
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API ServerRecievePacketEvent final : public NetworkEvent
	{
		String data;

		ServerRecievePacketEvent(const String & data)
			: NetworkEvent(EV_ServerRecievePacket)
			, data(data)
		{
		}
		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << data;
		}
	};


	// Client
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_NETWORK_API ClientRecievePacketEvent final : public NetworkEvent
	{
		String data;

		ClientRecievePacketEvent(const String & data)
			: NetworkEvent(EV_ClientRecievePacket)
			, data(data)
		{
		}
		inline void serialize(std::ostream & out) const override
		{
			out << "[" << get_type().name << "] " << data;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_NETWORK_EVENTS_HPP_
