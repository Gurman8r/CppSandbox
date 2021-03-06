#ifndef _ML_NETWORK_INTERFACE_HPP_
#define _ML_NETWORK_INTERFACE_HPP_

#include <MemeCore/IEventListener.hpp>
#include <MemeCore/IDisposable.hpp>
#include <MemeNet/GUID.hpp>
#include <MemeNet/IpAddress.hpp>
#include <MemeNet/Packet.hpp>
#include <MemeNet/SendSettings.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_LOCALHOST	"127.0.0.1"
#define ML_DEFAULT_PORT	60000
#define ML_MAX_CLIENTS	10

/* * * * * * * * * * * * * * * * * * * * */

#define ML_USER_PACKET 134 // ID_USER_PACKET_ENUM
#define ML_SERVER_RECIEVE (ML_USER_PACKET + 1)
#define ML_CLIENT_RECIEVE (ML_USER_PACKET + 2)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_NETWORK_API NetInterface
		: public IEventListener
		, public IDisposable
	{
	public:
		NetInterface();
		virtual ~NetInterface();

	public:
		bool setup();
		bool dispose() override;
		void poll();

	public:
		uint32_t send(const GUID & guid, const String & data, const SendSettings & settings);
		uint32_t send(const IpAddress & address, const String & data, const SendSettings & settings);

	public:
		virtual void onEvent(const IEvent * ev) override = 0;
		virtual void onPacket(const Packet & value) = 0;

	public:
		GUID		getMyGUID() const;
		GUID		getGUIDFromAddress(const IpAddress & value) const;
		IpAddress	getMyAddress() const;
		IpAddress	getAddressFromGUID(const GUID & value) const;

	protected:
		void * m_peer;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_INTERFACE_HPP_