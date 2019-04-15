#ifndef _ML_FTP_HPP_
#define _ML_FTP_HPP_

#include <MemeNet/GUID.hpp>
#include <MemeNet/IpAddress.hpp>
#include <MemeNet/Packet.hpp>
#include <MemeNet/SendSettings.hpp>

namespace ml
{
	// WIP
	class ML_NETWORK_API Ftp final
		: public ITrackable
		, public INonCopyable
	{
	public:
		Ftp();
		~Ftp();

	private:

	};
}

#endif // !_ML_FTP_HPP_