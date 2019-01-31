#ifndef _NETWORK_INTERFACE_H_
#define _NETWORK_INTERFACE_H_

#include <MemeNet/Export.hpp>
#include <MemeCore/EventListener.hpp>
#include <MemeNet/NetworkTypes.hpp>

#define ML_LOCALHOST "127.0.0.1"
#define ML_PORT 60000
#define ML_MAX_CLIENTS 10

#define ML_USER_PACKET 134 // ID_USER_PACKET_ENUM
#define ML_SERVER_RECIEVE (ML_USER_PACKET + 1)
#define ML_CLIENT_RECIEVE (ML_USER_PACKET + 2)

namespace ml
{
	class ML_NETWORK_API NetworkInterface
		: public EventListener
	{
	public:
		NetworkInterface();
		virtual ~NetworkInterface();

		bool setup();
		void cleanup();
		void poll();

		uint32_t send(const GUID & guid, const String & data, const SendSettings & settings);
		uint32_t send(const Address & address, const String & data, const SendSettings & settings);

		virtual void onEvent(const Event * ev) override = 0;
		virtual void onPacket(const Packet & value) = 0;

		Address getMyAddress() const;
		GUID	getMyGUID() const;
		Address getAddressFromGUID(const GUID & value) const;
		GUID	getGUIDFromAddress(const Address & value) const;


	protected:
		void *	m_peer;
	};
}

#endif // !_NETWORK_INTERFACE_H_
