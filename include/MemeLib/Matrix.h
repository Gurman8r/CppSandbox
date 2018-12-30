#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <MemeLib/ArrayIterator.h>
#include <initializer_list>

namespace ml
{
	template <typename T, std::size_t X, std::size_t Y>
	class Matrix
		: public ITrackable
		, public IComparable<Matrix<T, X, Y>>
	{
	public:
		using self_type = Matrix<T, X, Y>;

		static const std::size_t Cols = X;
		static const std::size_t Rows = Y;
		static const std::size_t Size = (Rows * Cols);

	private:
		T m_data[Size];

	public:
		using iterator			= ArrayIterator<std::forward_iterator_tag, T>;
		using const_iterator	= ArrayIterator<std::forward_iterator_tag, const T>;

		inline iterator			begin() { return iterator(&m_data[0]); }
		inline iterator			end()	{ return iterator(&m_data[Size]); }
		inline const_iterator	cbegin(){ return begin(); }
		inline const_iterator	cend()	{ return end(); }

	public:
		Matrix()
		{
			for (std::size_t y = 0; y < Rows; y++)
			{
				for (std::size_t x = 0; x < Cols; x++)
				{
					(*this)[y * Cols + x] = (x == y) ? (T)1 : (T)0;
				}
			}
		}
		Matrix(T value)
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value;
			}
		}
		Matrix(const T * value)
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}
		Matrix(T value[Size])
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}
		Matrix(const std::initializer_list<T> & value)
		{
			for (auto it = value.begin(); it != value.end(); it++)
			{
				(*this)[(it - value.begin())] = (*it);
			}
		}
		Matrix(const self_type & value)
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}
		virtual ~Matrix() {}

	public:
		inline T * ptr() const
		{
			return m_data;
		}
		
		inline const T & operator[](std::size_t index) const
		{
			assert(index < (*this).Size);
			return m_data[index];
		}
		
		inline T & operator[](std::size_t index)
		{
			assert(index < (*this).Size);
			return m_data[index];
		}

	public:
		inline virtual void serialize(std::ostream & out) const override
		{
			for (std::size_t y = 0; y < Rows; y++)
			{
				for (std::size_t x = 0; x < Cols; x++)
				{
					out << (*this)[y * Cols + x]
						<< (x < Cols - 1 ? ", " : "");
				}
				if (y < Rows - 1)
				{
					out << std::endl;
				}
			}
		}
		
		inline virtual void deserialize(std::istream & in) override
		{
		}
		
		inline virtual bool equals(const self_type & value) const override
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				if ((*this)[i] != value[i])
				{
					return false;
				}
			}
			return true;
		}
		
		inline virtual bool lessThan(const self_type & value) const override
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}
	};

	template <typename T> using Matrix3 = Matrix<T, 3, 3>;
	template <typename T> using Matrix4 = Matrix<T, 4, 4>;

	using Matrix3f = Matrix3<float>;
	using Matrix4f = Matrix4<float>;

	using mat3f = Matrix3f;
	using mat4f = Matrix4f;
}

#endif // !_MATRIX_H_
