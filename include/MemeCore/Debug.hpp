#ifndef _ML_DEBUG_HPP_
#define _ML_DEBUG_HPP_

#include <MemeCore/Console.hpp>
#include <MemeCore/String.hpp>
#include <MemeCore/ISingleton.hpp>

#define ML_WARNING -1 // | -1  | true  |
#define ML_FAILURE	0 // |  0  | false |
#define ML_SUCCESS	1 // | +1  | true  |

# ifdef ML_DEBUG
	// The do-while loop is needed so that mlCheck can be 
	// used as a single statement in if/else branches
#	define mlCheck(expr) do { expr; ml::Debug::checkError(__FILE__, __LINE__, #expr); } while (false)
# else
#	define mlCheck(expr) (expr)
# endif

namespace ml
{
	class ML_CORE_API Debug final
		: public ISingleton<Debug>
	{
		friend class ISingleton<Debug>;

		int32_t m_errorCode = ML_SUCCESS;
		String	m_errorName = String();
		String	m_errorDesc = String();

	public:
		static void	checkError(CString file, uint32_t line, CString expression);
		static void	setError(const int32_t code, const String & name, const String & desc);

	public:
		static int32_t	clear();
		static void		exit(int32_t exitCode);
		static int32_t	pause(int32_t exitCode);
		static int32_t	system(const char * cmd);
		static int32_t	system(const char * cmd, std::ostream & out);
		static void		terminate();

	public:
		static const String & platformTarget();
		static const String & configuration();

	public:
		static int32_t logWarning(const String & message); // - 1 true
		static int32_t logError(const String & message);   //   0 false
		static int32_t log(const String & message);		   // + 1 true

	public:
		template <class T, typename ... A>
		inline static int32_t logWarning(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::logWarning(fmt.format(arg0, (args)...));
		}

		template <class T, typename ... A>
		inline static int32_t logError(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::logError(fmt.format(arg0, (args)...));
		}

		template <class T, typename ... A>
		inline static int32_t log(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::log(fmt.format(arg0, (args)...));
		}
	};
	
}

#endif // !_ML_DEBUG_HPP_