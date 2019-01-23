#include <MemeNet/Client.h>
#include <MemeCore/DebugUtility.h>
#include <RakNet/RakPeerInterface.h>
#include <iostream>

namespace ml
{
	Client::Client()
	{
		using namespace RakNet;
		m_peer = static_cast<RakPeerInterface*>(RakPeerInterface::GetInstance());
		if (!m_peer)
		{
			Debug::LogError("Client: Failed Getting RakPeerInterface Instance");
		}
	}

	Client::~Client()
	{
	}
}