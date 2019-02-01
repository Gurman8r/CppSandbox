#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeNet/NetworkInterface.hpp>
#include <MemeNet/NetworkTypes.hpp>

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

#endif // !_SERVER_HPP_
