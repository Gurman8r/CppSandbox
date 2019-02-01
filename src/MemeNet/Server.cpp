#include <MemeNet/Server.hpp>
#include <MemeNet/NetworkEvents.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/EventSystem.hpp>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>

#define ML_PEER	static_cast<RakNet::RakPeerInterface *>

namespace ml
{
	Server::Server()
		: NetworkInterface()
		, m_running(false)
	{
		ML_EventSystem.addListener(NetworkEvent::EV_ServerRecievePacket, this);
	}

	Server::~Server()
	{
	}


	void Server::onEvent(const Event * value)
	{
		switch (value->eventID())
		{
		case NetworkEvent::EV_ServerRecievePacket:
			if (auto ev = value->As<ServerRecievePacketEvent>())
			{
				Debug::Log("SERVER -> {0}", (*ev));
			}
			break;
		}
	}

	void Server::onPacket(const Packet & value)
	{
		switch (value.data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			Debug::Log("Remote Disconnection");
			break;
	
		case ID_REMOTE_CONNECTION_LOST:
			Debug::LogError("Remote Connection List");
			break;
	
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			Debug::Log("Remote New Incoming Connection");
			break;
	
		case ID_NEW_INCOMING_CONNECTION:
			Debug::Log("New Incoming Connection");
			break;
	
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			Debug::LogError("No Free Incoming Connections");
			break;
	
		case ID_DISCONNECTION_NOTIFICATION:
			Debug::Log("Disconnected");
			break;
	
		case ID_CONNECTION_LOST:
			Debug::LogError("Connection Lost");
			break;
	
		case ML_SERVER_RECIEVE:
			RakNet::BitStream bitStream(value.data, value.size, false);
			RakNet::RakString str;
			if (bitStream.Read(str))
			{
				ML_EventSystem.fireEvent(ServerRecievePacketEvent(str.C_String()));
			}
			break;
		}
	}
	
	bool Server::start(const Address & addr, uint32_t maxClients)
	{
		if (m_peer && (m_maxClients = maxClients))
		{
			RakNet::SocketDescriptor socket(addr.port, addr.to_cstr());
			switch (ML_PEER(m_peer)->Startup(m_maxClients, &socket, 1))
			{
			case RakNet::RAKNET_STARTED:
				m_running = true;
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
	
}