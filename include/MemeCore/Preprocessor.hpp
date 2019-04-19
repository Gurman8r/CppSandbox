#ifndef _ML_PREPROCESSOR_HPP_
#define _ML_PREPROCESSOR_HPP_

// Stringify
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_xstr(a) ML_str(a)
#define ML_str(a) #a

// Assertions
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_assert_is_base_of(base, derived) \
static_assert( \
	std::is_base_of<base, derived>::value, \
	"" ML_str(derived) " must derive " ML_xstr(base) "" \
);

// Clamp
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_CLAMP(value, min, max) \
(((value) > (max)) \
	? (((value) < (min)) \
		? (min) \
		: (value)) \
	: (max))


// Lerp
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_LERP(a, b, c) (a * c + b * ((1) - c))


// Map Range
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_MAP_RANGE(value, min1, max1, min2, max2) \
(min2 + (value - min1) * (max2 - min2) / (max1 - min1))


// Sign
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_SIGN(value) \
(((value) == (0)) \
	? (0) \
	: (((value) > (0)) \
		? (+1) \
		: (-1)))


// Generate Mask Operators
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_MASK_OPERATORS(NAME, TYPE) \
inline NAME operator&(const NAME a, const NAME b) { return (NAME)((TYPE)a & (TYPE)b); } \
inline NAME operator|(const NAME a, const NAME b) { return (NAME)((TYPE)a | (TYPE)b); } \
inline NAME & operator &=(NAME & a, const NAME b) { return (a = (a & b)); } \
inline NAME & operator |=(NAME & a, const NAME b) { return (a = (a | b)); }


// Generate Iter Operators
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_ITER_OPERATORS_T(T, NAME, TYPE, ITER, MIN, MAX) \
template <class T> inline NAME operator+(const NAME a, const T b) \
{ \
	return (NAME)(ML_CLAMP(((TYPE)a + (TYPE)b), MIN, MAX)); \
} \
template <class T> inline NAME operator-(const NAME a, const T b) { return (a + (-b)); } \
template <class T> inline NAME & operator+=(NAME & a, const T b) { return (a = (a + b)); } \
template <class T> inline NAME & operator-=(NAME & a, const T b) { return (a = (a - b)); } \
inline NAME operator++(NAME & a)		{ return (a += 1); } \
inline NAME operator--(NAME & a)		{ return (a -= 1); } \
inline NAME operator++(NAME & a, ITER)	{ NAME c = a; a += 1; return c; } \
inline NAME operator--(NAME & a, ITER)	{ NAME c = a; a -= 1; return c; }

#define ML_GENERATE_ITER_OPERATORS(NAME, TYPE, ITER, MIN, MAX) \
ML_GENERATE_ITER_OPERATORS_T(T, NAME, TYPE, ITER, MIN, MAX)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PREPROCESSOR_HPP_