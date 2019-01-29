#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <MemeCore/ISingleton.h>
#include <MemeNet/NetworkInterface.h>

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
		void poll() override;
		void onEvent(const Event * value) override;
		
		bool connect(const std::string & addr, uint16_t port, const std::string & pass);

	public:
		inline bool connected() const { return m_connected; }

	private:
		bool m_connected;
	};
}

#endif // !_CLIENT_H_
