#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <MemeNet/Export.h>

namespace ml
{
	class ML_NETWORK_API Client
	{
	public:
		Client();
		~Client();

	private:
		void * m_peer;
	};
}

#endif // !_CLIENT_H_
