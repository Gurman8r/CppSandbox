#ifndef _DEBUG_UTILITY_H_
#define _DEBUG_UTILITY_H_

#include <MemeCore/Console.h>
#include <MemeCore/String.h>

namespace ml
{
	class ML_CORE_API Debug final
	{
	public:
		enum Status : int32_t
		{
			Warning = -1,
			Error	=  0,
			Success = +1,
		};

	public:
		static int32_t	clear();
		static void		exit(int32_t exitCode);
		static int32_t	pause(int32_t exitCode);
		static void		terminate(bool areYouSure);

	public:
		static Status LogWarning(const string & message);	// - 1 true
		static Status LogError(const string & message);		//   0 false
		static Status Log(const string & message);			// + 1 true

	public:
		template<typename T, typename ... A>
		inline static Status LogWarning(const string & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogWarning(string::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static Status LogError(const string & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogError(string::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static Status Log(const string & fmt, const T & arg0, const A &... args)
		{
			return Debug::Log(string::Format(fmt, arg0, (args)...));
		}

	public:
		inline static std::ostream & out()	{ return std::cout; }
		inline static std::ostream & err()	{ return std::cerr; }
		inline static std::istream & in()	{ return std::cin; }
	};

	static std::ostream & cout	= Debug::out();
	static std::ostream & cerr	= Debug::err();
	static std::istream & cin	= Debug::in();

	inline static std::ostream & endl(std::ostream & out)
	{
		return out << std::endl;
	}
	
}

#endif // !_DEBUG_UTILITY_H_
