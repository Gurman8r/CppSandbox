#include <MemeNet/Server.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>

namespace ml
{
	Server::Server()
	{
		m_peer = static_cast<RakNet::RakPeerInterface*>(RakNet::RakPeerInterface::GetInstance());
		if (!m_peer)
		{
			std::cerr << "Server: Failed Getting RakPeerInterface Instance" << std::endl;
		}
	}

	Server::~Server()
	{
	}
}