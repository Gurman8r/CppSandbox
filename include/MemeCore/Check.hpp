#ifndef _CHECK_HPP_
#define _CHECK_HPP_

// https://stackoverflow.com/questions/6534041/how-to-check-whether-operator-exists
// https://stackoverflow.com/questions/12509962/static-assert-for-ensuring-design-contract

#define ML_OP_CHECK(OP, NAME) \
class no_##NAME##_operator \
{ \
	bool b[2]; \
}; \
template<typename T, typename Arg> \
no_##NAME##_operator OP(const T &, const Arg &); \
bool check_##NAME##_operator(...); \
no_##NAME##_operator & check_##NAME##_operator(const no_##NAME##_operator &); \
template <typename T, typename Arg = T> \
struct has_##NAME##_operator \
{ \
	enum \
	{ \
		value = ( \
			sizeof(check_##NAME##_operator(*(T*)(0) == *(Arg*)(0))) != \
			sizeof(no_##NAME##_operator)) \
	}; \
} \

namespace ml
{
	namespace check
	{
		ML_OP_CHECK(operator<<, leftshift);
		ML_OP_CHECK(operator>>, rightshift);
	}
}

#endif // !_CHECK_HPP_
