#ifndef _ML_FILE_DETAILS_HPP_
#define _ML_FILE_DETAILS_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_CORE_API FileDetails
		: public ITrackable
	{
	public:
		FileDetails() {}
		
		virtual ~FileDetails() {}
	};
}

#endif