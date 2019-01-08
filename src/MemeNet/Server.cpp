#include <MemeNet/Server.h>
#include <MemeCore/DebugUtility.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>

namespace ml
{
	Server::Server()
	{
		m_peer = static_cast<RakNet::RakPeerInterface*>(RakNet::RakPeerInterface::GetInstance());
		if (!m_peer)
		{
			Debug::LogError("Server: Failed Getting RakPeerInterface Instance");
		}
	}

	Server::~Server()
	{
	}
}