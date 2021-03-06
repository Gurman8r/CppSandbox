#include <MemeNet/NetInterface.hpp>
#include <MemeNet/NetworkEvents.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeNet/RakNet.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	NetInterface::NetInterface()
		: m_peer(NULL)
	{
	}
	
	NetInterface::~NetInterface()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool NetInterface::setup()
	{
		return ((!m_peer) && (m_peer = ML_PEER(RakNet::RakPeerInterface::GetInstance())));
	}

	bool NetInterface::dispose()
	{
		if (m_peer)
		{
			RakNet::RakPeerInterface::DestroyInstance(ML_PEER(m_peer));
		}
		return (!m_peer);
	}

	void NetInterface::poll()
	{
		for (void * packet = ML_PEER(m_peer)->Receive();
			(packet != NULL);
			(ML_PEER(m_peer)->DeallocatePacket(ML_PACKET(packet))),
			(packet = ML_PEER(m_peer)->Receive()))
		{
			onPacket(Packet({
					ML_PACKET(packet)->systemAddress.ToString(),
					ML_PACKET(packet)->systemAddress.GetPort()
				},
				ML_PACKET(packet)->guid.g,
				ML_PACKET(packet)->length,
				ML_PACKET(packet)->data
			));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t NetInterface::send(const GUID & guid, const String & data, const SendSettings & settings)
	{
		RakNet::BitStream bitStream;
		bitStream.Write(data.c_str());
		return ML_PEER(m_peer)->Send(
			&bitStream,
			(PacketPriority)settings.priority,
			(PacketReliability)settings.reliability,
			settings.ordering,
			ML_PEER(m_peer)->GetSystemAddressFromGuid(RakNet::RakNetGUID(guid)),
			settings.broadcast,
			settings.receiptNumber);
	}

	uint32_t NetInterface::send(const IpAddress & addr, const String & data, const SendSettings & settings)
	{
		RakNet::BitStream bitStream;
		bitStream.Write(data.c_str());
		return ML_PEER(m_peer)->Send(
			&bitStream,
			(PacketPriority)settings.priority,
			(PacketReliability)settings.reliability,
			settings.ordering,
			RakNet::SystemAddress(addr.ToCString(), addr.port),
			settings.broadcast,
			settings.receiptNumber);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	GUID NetInterface::getMyGUID() const
	{
		return GUID(
			ML_PEER(m_peer)->GetMyGUID().g
		);
	}

	GUID NetInterface::getGUIDFromAddress(const IpAddress & value) const
	{
		return GUID(
			ML_PEER(m_peer)->GetGuidFromSystemAddress(
				RakNet::SystemAddress(value.ToCString(), value.port)
			).g
		);
	}

	IpAddress NetInterface::getMyAddress() const
	{
		return getAddressFromGUID(getMyGUID());
	}

	IpAddress NetInterface::getAddressFromGUID(const GUID & value) const
	{
		const auto addr = ML_PEER(m_peer)->GetSystemAddressFromGuid(
			RakNet::RakNetGUID(value)
		);
		return IpAddress(
			addr.ToString(),
			addr.GetPort()
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}