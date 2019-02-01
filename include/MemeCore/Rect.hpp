#ifndef _RECT_HPP_
#define _RECT_HPP_

#include <MemeCore/Vector2.hpp>
#include <MemeCore/Vector4.hpp>

namespace ml
{

	template <typename T>
	class Rect final
		: public Vector4<T>
	{
	public:
		using Value	= T;
		using Base		= Vector4<T>;
		using Self		= Rect<Value>;
		using coord_type	= Vector2<Value>;

		const static Self Empty;
		const static Self One;

	public:
		Rect()
			: Base()
		{
		}
		Rect(const Value & width, const Value & height)
			: Base(0, 0, width, height)
		{
		}
		Rect(const Value & left, const Value & top, const Value & width, const Value & height)
			: Base(left, top, width, height)
		{
		}
		Rect(const Self & copy)
			: Base(copy)
		{
		}
		Rect(const coord_type & position, const coord_type & size)
			: Base(position[0], position[1], size[0], size[1])
		{
		}
		Rect(const coord_type & size)
			: Base(0, 0, size[0], size[1])
		{
		}
		~Rect() 
		{
		}
		
	public:
		inline const Value left() const
		{
			return (*this)[0];
		}
		inline const Value top() const
		{
			return (*this)[1];
		}
		inline const Value width() const
		{
			return (*this)[2];
		}
		inline const Value height() const
		{
			return (*this)[3];
		}		
		inline const Value bot() const
		{
			return top() + height();
		}
		inline const Value right() const
		{
			return left() + width();
		}
		inline const coord_type position() const
		{
			return coord_type(left(), top());
		}
		inline const coord_type size() const
		{
			return coord_type(width(), height());
		}
		inline const coord_type center() const
		{
			return position() + (size() / Value(2));
		}
		
		inline Self &	left(Value value)
		{
			(*this)[0] = value;
			return (*this);
		}
		inline Self &	top(Value value)
		{
			(*this)[1] = value;
			return (*this);
		}
		inline Self &	width(Value value)
		{
			(*this)[2] = value;
			return (*this);
		}
		inline Self &	height(Value value)
		{
			(*this)[3] = value;
			return (*this);
		}		
		inline Self &	bot(Value value)
		{
			return height(value - top());
		}
		inline Self &	right(Value value)
		{
			return width(value - left());
		}
		inline Self &	position(const coord_type& value)
		{
			return left(value[0]).top(value[1]);
		}
		inline Self &	size(const coord_type& value)
		{
			return width(value[0]).height(value[1]);
		}
		inline Self &	center(const coord_type& value)
		{
			return position(value - (size() / Value(2)));
		}
		
		inline operator Base() const
		{
			return Base((*this)[0], (*this)[1], (*this)[2], (*this)[3]);
		}
	};

	template <typename T> const Rect<T> Rect<T>::Empty = Rect<T>(0, 0, 0, 0);
	template <typename T> const Rect<T> Rect<T>::One = Rect<T>(1, 1, 1, 1);

	using FloatRect = Rect<float>;
	using IntRect	= Rect<int32_t>;
	using UintRect	= Rect<uint32_t>;
}

#endif // !_RECT_HPP_
