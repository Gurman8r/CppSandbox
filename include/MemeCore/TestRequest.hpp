#ifndef _ML_REQUEST_HPP_
#define _ML_REQUEST_HPP_

#include <MemeCore/IRequest.hpp>

namespace ml
{
	class ML_CORE_API TestRequest
		: public IRequest
	{
	public:
		TestRequest();
		~TestRequest();

	public:
		void process() override;
		void finish() override;
	};
}

#endif // !_ML_REQUEST_HPP_