#ifndef _ML_RECT_HPP_
#define _ML_RECT_HPP_

#include <MemeCore/Vector4.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Elem
	> class Rect final
		: public Vector4<_Elem>
	{
	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type		= typename _Elem;
		using base_type			= typename Vector4<value_type>;
		using self_type			= typename Rect<value_type>;
		using coord_type		= typename Vector<value_type, 2>;

		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;

	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Rect()
			: base_type()
		{
		}
		
		Rect(const_reference width, const_reference height)
			: base_type(0, 0, width, height)
		{
		}
		
		Rect(const_reference left, const_reference top, const_reference width, const_reference height)
			: base_type(left, top, width, height)
		{
		}
		
		Rect(const self_type & copy)
			: base_type(copy)
		{
		}
		
		Rect(const coord_type & pos, const coord_type & size)
			: base_type(pos[0], pos[1], size[0], size[1])
		{
		}
		
		Rect(const coord_type & size)
			: base_type(0, 0, size[0], size[1])
		{
		}
		
		~Rect() {}

	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline value_type left()	const { return (*this)[0]; }
		inline value_type top()		const { return (*this)[1]; }
		inline value_type width()	const { return (*this)[2]; }
		inline value_type height()	const { return (*this)[3]; }

		inline value_type bot()		const { return (top() + height()); }
		inline value_type right()	const { return (left() + width()); }
		inline coord_type position()const { return { left(), top() }; }
		inline coord_type size()	const { return { width(), height() }; }
		inline coord_type center()	const { return (position() + (size() / static_cast<value_type>(2))); }

		/* * * * * * * * * * * * * * * * * * * * */

		inline self_type & left		(value_type value)	{ ((*this)[0] = value); return (*this); }
		inline self_type & top		(value_type value)	{ ((*this)[1] = value); return (*this); }
		inline self_type & width	(value_type value)	{ ((*this)[2] = value); return (*this); }
		inline self_type & height	(value_type value)	{ ((*this)[3] = value); return (*this); }

		inline self_type & bot		(value_type value)			{ return height(value - top()); }
		inline self_type & right	(value_type value)			{ return width(value - left()); }
		inline self_type & position	(const coord_type & value)	{ return left(value[0]).top(value[1]); }
		inline self_type & size		(const coord_type & value)	{ return width(value[0]).height(value[1]); }
		inline self_type & center	(const coord_type & value)	{ return position(value - (size() / static_cast<value_type>(2))); }


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline operator base_type() const
		{
			return base_type { (*this)[0], (*this)[1], (*this)[2], (*this)[3] };
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using FloatRect = Rect<float>;
	using IntRect	= Rect<int32_t>;
	using UintRect	= Rect<uint32_t>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RECT_HPP_