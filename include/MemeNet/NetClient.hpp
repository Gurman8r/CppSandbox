#ifndef _ML_CLIENT_HPP_
#define _ML_CLIENT_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeNet/NetInterface.hpp>

#define ML_NetClient ml::Client::getInstance()

namespace ml
{
	class ML_NETWORK_API Client final
		: public ITrackable
		, public NetInterface
		, public ISingleton<Client>
	{
		friend class ISingleton<Client>;

	private:
		Client();
		~Client();

	public:
		void onEvent(const IEvent * value) override;
		
		void onPacket(const Packet & value) override;

		bool connect(const IpAddress & addr, const String & pass = String());

	public:
		inline bool connected() const { return m_connected; }

	private:
		bool m_connected;
	};
}

#endif // !_ML_CLIENT_HPP_