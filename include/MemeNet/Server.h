#ifndef _SERVER_H_
#define _SERVER_H_

#include <MemeCore/ISingleton.h>
#include <MemeNet/NetworkInterface.h>
#include <MemeNet/NetworkTypes.h>

#define ML_Server ml::Server::getInstance()

namespace ml
{
	class ML_NETWORK_API Server final
		: public ITrackable
		, public NetworkInterface
		, public ISingleton<Server>
	{
		friend class ISingleton<Server>;

	private:
		Server();
		~Server();

	public:
		void onEvent(const Event * value) override;

		void onPacket(const Packet & value) override;

		bool start(const Address & addr, uint32_t maxClients);

	public:
		inline bool		running()		const { return m_running; }
		inline uint32_t maxClients()	const { return m_maxClients; }

	private:
		bool		m_running;
		uint32_t	m_maxClients;
	};
}

#endif // !_SERVER_H_
