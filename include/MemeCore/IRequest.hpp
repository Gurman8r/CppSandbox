#ifndef _ML_I_REQUEST_HPP_
#define _ML_I_REQUEST_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_CORE_API IRequest
		: public ITrackable
	{
	public:
		virtual void process() = 0;
		virtual void finish() = 0;
	};
}

#endif // !_ML_I_REQUEST_HPP_