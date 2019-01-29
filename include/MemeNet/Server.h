#ifndef _SERVER_H_
#define _SERVER_H_

#include <MemeCore/ISingleton.h>
#include <MemeNet/NetworkInterface.h>

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
		void poll() override;
		void onEvent(const Event * value) override;

	public:
		inline bool running() const { return m_running; }

	private:
		bool m_running;
	};
}

#endif // !_SERVER_H_
