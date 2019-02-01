#include <MemeNet/Client.hpp>
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
	Client::Client()
		: NetworkInterface()
		, m_connected(false)
	{
		ML_EventSystem.addListener(NetworkEvent::EV_ClientRecievePacket, this);
	}

	Client::~Client()
	{
	}

	
	void Client::onEvent(const Event * value)
	{
		switch (value->eventID())
		{
		case NetworkEvent::EV_ClientRecievePacket:
			if (auto ev = value->Cast<ClientRecievePacketEvent>())
			{
				Debug::Log("CLIENT -> {0}", (*ev));
			}
			break;
		}
	}

	void Client::onPacket(const Packet & value)
	{
		switch (value.data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			Debug::Log("Connection Request Accepted");
			m_connected = true;
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

		case ML_CLIENT_RECIEVE:
			RakNet::BitStream bitStream(value.data, value.size, false);
			RakNet::RakString str;
			if (bitStream.Read(str))
			{
				ML_EventSystem.fireEvent(ClientRecievePacketEvent(str.C_String()));
			}
			break;
		}
	}

	bool Client::connect(const Address & addr, const String & pass)
	{
		if (m_peer)
		{
			switch (ML_PEER(m_peer)->Connect(
				addr.to_cstr(),
				addr.port,
				pass.c_str(),
				(int32_t)pass.size()))
			{
			case RakNet::CONNECTION_ATTEMPT_STARTED:
				return Debug::Log("CONNECTION_ATTEMPT_STARTED");

			case RakNet::INVALID_PARAMETER:
				return Debug::LogError("INVALID_PARAMETER");

			case RakNet::CANNOT_RESOLVE_DOMAIN_NAME:
				return Debug::LogError("CANNOT_RESOLVE_DOMAIN_NAME");

			case RakNet::ALREADY_CONNECTED_TO_ENDPOINT:
				return Debug::LogError("ALREADY_CONNECTED_TO_ENDPOINT");

			case RakNet::CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
				return Debug::LogError("CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS");

			case RakNet::SECURITY_INITIALIZATION_FAILED:
				return Debug::LogError("SECURITY_INITIALIZATION_FAILED");
			}
		}
		return false;
	}

}