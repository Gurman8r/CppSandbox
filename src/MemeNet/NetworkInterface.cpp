#include <MemeNet/NetworkInterface.h>
#include <MemeNet/NetworkEvents.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/EventSystem.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>

#define ML_PEER		static_cast<RakNet::RakPeerInterface *>
#define ML_PACKET	static_cast<RakNet::Packet *>

namespace ml
{
	NetworkInterface::NetworkInterface()
		: m_peer(NULL)
	{
	}
	
	NetworkInterface::~NetworkInterface()
	{
		cleanup();
	}


	bool NetworkInterface::setup()
	{
		return (!m_peer && (m_peer = ML_PEER(RakNet::RakPeerInterface::GetInstance())));
	}

	void NetworkInterface::cleanup()
	{
		if (m_peer)
		{
			RakNet::RakPeerInterface::DestroyInstance(ML_PEER(m_peer));
		}
	}

	void NetworkInterface::poll()
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


	uint32_t NetworkInterface::send(const GUID & guid, const std::string & data, const SendSettings & settings)
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

	uint32_t NetworkInterface::send(const Address & addr, const std::string & data, const SendSettings & settings)
	{
		RakNet::BitStream bitStream;
		bitStream.Write(data.c_str());
		return ML_PEER(m_peer)->Send(
			&bitStream,
			(PacketPriority)settings.priority,
			(PacketReliability)settings.reliability,
			settings.ordering,
			RakNet::SystemAddress(addr.addr.c_str(), addr.port),
			settings.broadcast,
			settings.receiptNumber);
	}

}