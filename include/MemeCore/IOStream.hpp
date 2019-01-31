#ifndef _ML_OSTREAM_H_
#define _ML_OSTREAM_H_

#include <MemeCore/Export.hpp>
#include <iostream>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API IOStream final
	{
	public:
		inline static std::ostream & out() { return std::cout; }
		inline static std::ostream & err() { return std::cerr; }
		inline static std::istream & in() { return std::cin; }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static std::ostream & cout = IOStream::out();
	static std::ostream & cerr = IOStream::err();
	static std::istream & cin = IOStream::in();

	inline static std::ostream & endl(std::ostream & out)
	{
		return out << std::endl;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API BasicOStream
		: public std::ostream
		, public std::streambuf
	{
	public:
		using base_type = std::ostream;

	public:
		BasicOStream()
			: base_type(this)
		{
		}
		~BasicOStream() {}

		virtual int32_t overflow(int32_t c) override
		{
			return 0;
		}

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_OSTREAM_H_