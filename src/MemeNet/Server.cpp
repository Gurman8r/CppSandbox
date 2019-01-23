#include <MemeNet/Server.h>
#include <MemeCore/DebugUtility.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>

namespace ml
{
	Server::Server()
	{
		using namespace RakNet;
		m_peer = static_cast<RakPeerInterface*>(RakPeerInterface::GetInstance());
		if (!m_peer)
		{
			Debug::LogError("Server: Failed Getting RakPeerInterface Instance");
		}
	}

	Server::~Server()
	{
	}
}