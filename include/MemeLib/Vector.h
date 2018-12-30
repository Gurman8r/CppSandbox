#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <MemeLib/Matrix.h>

namespace ml
{
	template <typename T, std::size_t N>
	class Vector
		: public Matrix<T, N, 1>
	{
	public:
		virtual ~Vector() {}

	};
}

#endif // !_VECTOR_H_
