#include <MemeNet/Client.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>

namespace ml
{
	Client::Client()
	{
		m_peer = static_cast<RakNet::RakPeerInterface*>(RakNet::RakPeerInterface::GetInstance());
		if (!m_peer)
		{
			std::cerr << "Client: Failed Getting RakPeerInterface Instance" << std::endl;
		}
	}

	Client::~Client()
	{
	}
}