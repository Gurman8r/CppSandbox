#ifndef _ML_PREPROCESSOR_HPP_
#define _ML_PREPROCESSOR_HPP_

// Clamp Macro
/* * * * * * * * * * * * * * * * * * * * */
#define ML_CLAMP(value, min, max) \
(((value) > (max)) \
	? (((value) < (min)) \
		? (min) \
		: (value)) \
	: (max)) \


// Generate Mask Operators Macro
/* * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_MASK_OPERATORS(NAME, TYPE) \
inline NAME operator&(const NAME l, const NAME r) { return (NAME)((TYPE)l & (TYPE)r); } \
inline NAME operator|(const NAME l, const NAME r) { return (NAME)((TYPE)l | (TYPE)r); } \
inline NAME & operator &=(NAME & l, const NAME r) { return (l = (l & r)); } \
inline NAME & operator |=(NAME & l, const NAME r) { return (l = (l | r)); } \


// Generate Iterable Operators Macro
/* * * * * * * * * * * * * * * * * * * * */
#define ML_GENERATE_ITERABLE_OPERATORS(NAME, TYPE, ITER, MIN, MAX) \
template <class T> inline NAME operator+(const NAME l, const T r) \
{ \
	return (NAME)(ML_CLAMP(((TYPE)l + (TYPE)r), MIN, MAX)); \
} \
template <class T> inline NAME operator-(const NAME l, const T r) { return (l + (-r)); } \
template <class T> inline NAME & operator+=(NAME & l, const T r) { return (l = (l + r)); } \
template <class T> inline NAME & operator-=(NAME & l, const T r) { return (l = (l - r)); } \
inline NAME operator++(NAME & l) { return (l += 1); } \
inline NAME operator++(NAME & l, ITER) { NAME temp = l;	l += 1;	return temp; } \
inline NAME operator--(NAME & l) { return (l -= 1); } \
inline NAME operator--(NAME & l, ITER) { NAME temp = l;	l -= 1;	return temp; } \

#endif // !_ML_PREPROCESSOR_HPP_