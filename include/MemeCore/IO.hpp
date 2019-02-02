#ifndef _ML_IO_HPP_
#define _ML_IO_HPP_

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

	template <class E, class T>
	inline std::basic_ostream<E, T> & endl(std::basic_ostream<E, T> & out)
	{
		out.put(out.widen('\n'));
		out.flush();
		return (out);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IO_HPP_
