#ifndef _ML_I_READABLE_HPP_
#define _ML_I_READABLE_HPP_

#include <MemeCore/IDisposable.hpp>
#include <MemeCore/String.hpp>

namespace ml
{
	class IReadable
		: public IDisposable
	{
	public:
		virtual bool cleanup() = 0;
		virtual bool loadFromFile(const String & filename) = 0;
	};
}

#endif // !_ML_I_READABLE_HPP_