#include <MemeNet/Server.h>
#include <MemeNet/NetworkEvents.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/EventSystem.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>

#define ML_SERVER_RECIEVE (ID_USER_PACKET_ENUM + 1)
#define ML_CLIENT_RECIEVE (ID_USER_PACKET_ENUM + 2)

#define ML_PEER		static_cast<RakNet::RakPeerInterface*>
#define ML_PACKET	static_cast<RakNet::Packet *>

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


	void Server::poll()
	{
		if (!m_running)
			return;

		for (void * packet = ML_PEER(m_peer)->Receive();
			(packet != NULL);
			(ML_PEER(m_peer)->DeallocatePacket(ML_PACKET(packet))),
			(packet = ML_PEER(m_peer)->Receive()))
		{
			switch (ML_PACKET(packet)->data[0])
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
			{
				RakNet::BitStream bitStream(
					ML_PACKET(packet)->data,
					ML_PACKET(packet)->length,
					false);

				RakNet::RakString str;
				if (bitStream.Read(str))
				{
					ML_EventSystem.fireEvent(ServerRecievePacketEvent(str.C_String()));
				}
			}
			break;

			case ML_CLIENT_RECIEVE:
			{
				RakNet::BitStream bitStream(
					ML_PACKET(packet)->data,
					ML_PACKET(packet)->length,
					false);

				RakNet::RakString str;
				if (bitStream.Read(str))
				{
					ML_EventSystem.fireEvent(ClientRecievePacketEvent(str.C_String()));
				}
			}
			break;
			}
		}
	}

	void Server::onEvent(const Event * value)
	{
		switch (value->eventID())
		{
		case NetworkEvent::EV_ServerRecievePacket:
			if (auto ev = value->As<ServerRecievePacketEvent>())
			{
				Debug::Log("Server Recieve Packet Event");
			}
			break;
		}
	}

	
}