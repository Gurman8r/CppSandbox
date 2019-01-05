#include <MemeNet/Client.h>
#include <RakNet/RakPeerInterface.h>

namespace ml
{
	Client::Client()
	{
		using namespace RakNet;
		m_peer = static_cast<RakPeerInterface*>(RakPeerInterface::GetInstance());
	}

	Client::~Client()
	{
	}
}