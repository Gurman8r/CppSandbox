#ifndef _ML_IO_H_
#define _ML_IO_H_

#include <MemeCore/Export.hpp>
#include <MemeCore/STD.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API IO final
	{
	public:
		inline static std::ostream & out() { return std::cout; }
		inline static std::ostream & err() { return std::cerr; }
		inline static std::istream & in()  { return std::cin;  }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static std::ostream & cout	= IO::out();
	static std::ostream & cerr	= IO::err();
	static std::istream & cin	= IO::in();

	/* * * * * * * * * * * * * * * * * * * * */

	inline static std::ostream & endl(std::ostream & out)
	{
		return out << std::endl;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IO_H_