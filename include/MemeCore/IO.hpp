#ifndef _ML_IO_HPP_
#define _ML_IO_HPP_

#include <MemeCore/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using StreamBuf = typename std::streambuf;

	/* * * * * * * * * * * * * * * * * * * * */

	static std::ostream & cout = std::cout;
	static std::ostream & cerr = std::cerr;
	static std::istream & cin = std::cin;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API IO final
	{
		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Fun, 
			class ... Args
		> inline static StreamBuf * capture(std::ostream & src, Fun fun, Args ... args)
		{
			SStream dst;
			if (StreamBuf * buf = src.rdbuf(dst.rdbuf()))
			{
				fun(dst, (args)...);

				return src.rdbuf(buf);
			}
			return NULL;
		}

		template <
			class Fun,
			class ... Args
		> inline static StreamBuf * capture_cout(Fun fun, Args ... args)
		{
			return capture(cout, fun, (args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem, 
		class _Traits
	> inline std::basic_ostream<_Elem, _Traits> & endl(std::basic_ostream<_Elem, _Traits> & out)
	{
		out.put(out.widen('\n'));
		out.flush();
		return (out);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IO_HPP_