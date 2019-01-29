#ifndef _NETWORK_INTERFACE_H_
#define _NETWORK_INTERFACE_H_

#include <MemeNet/Export.h>
#include <MemeCore/EventListener.h>
#include <MemeNet/NetworkTypes.h>

#ifndef ML_LOCALHOST
#define ML_LOCALHOST "127.0.0.1"
#endif // !ML_LOCALHOST

#ifndef ML_PORT
#define ML_PORT 60000
#endif // !ML_PORT

namespace ml
{
	class ML_NETWORK_API NetworkInterface
		: public EventListener
	{
	public:
		NetworkInterface();
		virtual ~NetworkInterface();

		virtual bool setup();
		virtual void cleanup();

		virtual void poll() = 0;
		virtual void onEvent(const Event * ev) override = 0;

		uint32_t send(uint64_t guid, const char * data, const SendSettings & settings);
		uint32_t send(const char * addr, const char * data, const SendSettings & settings);

		Packet * recieve();
		
	protected:
		void *	m_peer;
	};
}

#endif // !_NETWORK_INTERFACE_H_
