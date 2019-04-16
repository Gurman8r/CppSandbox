#ifndef _ML_SERVER_HPP_
#define _ML_SERVER_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeNet/NetInterface.hpp>

#define ML_NetServer ml::Server::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_NETWORK_API Server final
		: public ITrackable
		, public NetInterface
		, public ISingleton<Server>
	{
		friend class ISingleton<Server>;

	private:
		Server();
		~Server();

	public:
		void onEvent(const IEvent * value) override;

		void onPacket(const Packet & value) override;

		bool start(const IpAddress & addr, uint32_t maxClients);

	public:
		inline bool		running()		const { return m_running; }
		inline uint32_t maxClients()	const { return m_maxClients; }

	private:
		bool		m_running;
		uint32_t	m_maxClients;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SERVER_HPP_