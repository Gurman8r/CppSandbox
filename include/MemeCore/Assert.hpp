#ifndef _ML_ASSERT_HPP_
#define _ML_ASSERT_HPP_

#include <MemeCore/Debug.hpp>

# ifdef ML_DEBUG
#	define ML_Assert(expr) do { expr; ml::Assert::checkError(__FILE__, __LINE__, #expr); } while (false)
# else
#	define ML_Assert(expr) (expr)
# endif

namespace ml
{
	struct ML_CORE_API Assert
	{
		static void	checkError(CString file, uint32_t line, CString expression);
	};
}

#endif // !_ML_ASSERT_HPP_