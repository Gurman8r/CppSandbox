#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeNet/NetworkInterface.hpp>

#define ML_Client ml::Client::getInstance()

namespace ml
{
	class ML_NETWORK_API Client final
		: public ITrackable
		, public NetworkInterface
		, public ISingleton<Client>
	{
		friend class ISingleton<Client>;

	private:
		Client();
		~Client();

	public:
		void onEvent(const IEvent * value) override;
		
		void onPacket(const Packet & value) override;

		bool connect(const Address & addr, const String & pass = String());

	public:
		inline bool connected() const { return m_connected; }

	private:
		bool m_connected;
	};
}

#endif // !_CLIENT_HPP_
