#include <MemeNet/NetworkInterface.h>
#include <MemeNet/NetworkEvents.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/EventSystem.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>

#define ML_PEER		static_cast<RakNet::RakPeerInterface*>
#define ML_PACKET	static_cast<RakNet::Packet *>

namespace ml
{
	NetworkInterface::NetworkInterface()
		: m_peer(NULL)
	{
	}
	
	NetworkInterface::~NetworkInterface()
	{
	}

	bool NetworkInterface::setup()
	{
		if (!m_peer && (m_peer = ML_PEER(RakNet::RakPeerInterface::GetInstance())))
		{
			RakNet::SocketDescriptor socket;

			switch (ML_PEER(m_peer)->Startup(1, &socket, 1))
			{
			case RakNet::RAKNET_STARTED:
				return Debug::Log("RAKNET_STARTED");

			case RakNet::RAKNET_ALREADY_STARTED:
				return Debug::LogWarning("RAKNET_ALREADY_STARTED");

			case RakNet::INVALID_SOCKET_DESCRIPTORS:
				return Debug::LogError("INVALID_SOCKET_DESCRIPTORS");

			case RakNet::INVALID_MAX_CONNECTIONS:
				return Debug::LogError("INVALID_MAX_CONNECTIONS");

			case RakNet::SOCKET_FAMILY_NOT_SUPPORTED:
				return Debug::LogError("SOCKET_FAMILY_NOT_SUPPORTED");

			case RakNet::SOCKET_PORT_ALREADY_IN_USE:
				return Debug::LogError("SOCKET_PORT_ALREADY_IN_USE");

			case RakNet::SOCKET_FAILED_TO_BIND:
				return Debug::LogError("SOCKET_FAILED_TO_BIND");

			case RakNet::SOCKET_FAILED_TEST_SEND:
				return Debug::LogError("SOCKET_FAILED_TEST_SEND");

			case RakNet::PORT_CANNOT_BE_ZERO:
				return Debug::LogError("PORT_CANNOT_BE_ZERO");

			case RakNet::FAILED_TO_CREATE_NETWORK_THREAD:
				return Debug::LogError("FAILED_TO_CREATE_NETWORK_THREAD");

			case RakNet::COULD_NOT_GENERATE_GUID:
				return Debug::LogError("COULD_NOT_GENERATE_GUID");

			case RakNet::STARTUP_OTHER_FAILURE:
			default:
				return Debug::LogError("STARTUP_OTHER_FAILURE");
			}
		}
		return false;
	}

	void NetworkInterface::cleanup()
	{
		if (m_peer)
		{
			RakNet::RakPeerInterface::DestroyInstance(ML_PEER(m_peer));
		}
	}


	uint32_t NetworkInterface::send(uint64_t guid, const char * data, const SendSettings & settings)
	{
		RakNet::BitStream bitStream;
		bitStream.Write(data);
		ML_PEER(m_peer)->Send(
			&bitStream,
			(PacketPriority)settings.priority,
			(PacketReliability)settings.reliability,
			settings.ordering,
			RakNet::RakNetGUID(guid),
			settings.broadcast,
			settings.receiptNumber);
	}

	uint32_t NetworkInterface::send(const char * addr, const char * data, const SendSettings & settings)
	{
		RakNet::BitStream bitStream;
		bitStream.Write(data);
		ML_PEER(m_peer)->Send(
			&bitStream,
			(PacketPriority)settings.priority,
			(PacketReliability)settings.reliability,
			settings.ordering,
			RakNet::SystemAddress(addr),
			settings.broadcast,
			settings.receiptNumber);
	}

	Packet * NetworkInterface::recieve()
	{
		if (RakNet::Packet * temp = ML_PEER(m_peer)->Receive())
		{
			return new Packet{
				Address(
					temp->systemAddress.ToString(), 
					temp->systemAddress.GetPort()),
				GUID(temp->guid.g),
				temp->length,
				temp->data
			};
		}
		return NULL;
	}
}