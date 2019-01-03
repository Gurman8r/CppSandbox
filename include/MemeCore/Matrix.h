#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <algorithm>
#include <MemeCore/IEnumerable.h>
#include <MemeCore/IComparable.h>
#include <MemeCore/Maths.h>
#include <initializer_list>

namespace ml
{
	// Template Fixed 2D Array
	template <typename T, std::size_t _Cols, std::size_t _Rows>
	class Matrix
		: public ITrackable
		, public IComparable<Matrix<T, _Cols, _Rows>>
		, public IEnumerable<T>

	{
	public:
		using value_type	= T;
		using self_type		= Matrix<value_type, _Cols, _Rows>;

		static const std::size_t Cols = _Cols;
		static const std::size_t Rows = _Rows;
		static const std::size_t Size = (Rows * Cols);

	private:
		value_type m_data[Size];

	public:
		Matrix()
			: IEnumerable<value_type>(&m_data[0], &m_data[Size])
		{
		}
		
		Matrix(const value_type & value)
			: self_type()
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value;
			}
		}
		
		Matrix(const value_type * value)
			: self_type()
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}
		
		Matrix(const std::initializer_list<value_type> & value)
			: self_type()
		{
			for (auto it = value.begin(); it != value.end(); it++)
			{
				(*this)[(it - value.begin())] = (*it);
			}
		}
		
		Matrix(const self_type & value)
			: self_type()
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}
		
		template <typename U, std::size_t C, std::size_t R>
		Matrix(const Matrix<U, C, R> & copy)
			: self_type()
		{
			for (std::size_t i = 0, imax = std::min<std::size_t>(Size, copy.Size); i < imax; i++)
			{
				(*this)[i] = static_cast<value_type>(copy[i]);
			}
		}

		virtual ~Matrix() {}

	public:
		inline value_type * ptr() const
		{
			return m_data;
		}
		
		inline const value_type & operator[](std::size_t index) const
		{
			assert(index < (*this).Size);
			return m_data[index];
		}
		
		inline value_type & operator[](std::size_t index)
		{
			assert(index < (*this).Size);
			return m_data[index];
		}

		inline static self_type identity()
		{
			self_type value;
			for (std::size_t y = 0; y < Rows; y++)
			{
				for (std::size_t x = 0; x < Cols; x++)
				{
					value[y * Cols + x] = (x == y) ? (value_type)1 : (value_type)0;
				}
			}
			return value;
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
			for (std::size_t i = 0; i < Size; i++)
			{
				in >> (*this)[i];
			}
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

	template <typename T, std::size_t N> 
	using MatrixN = Matrix<T, N, N>;
	
	template <typename T> 
	using Matrix3 = MatrixN<T, 3>;
	
	template <typename T> 
	using Matrix4 = MatrixN<T, 4>;

	using mat3f = Matrix3<float>;
	using mat4f = Matrix4<float>;
}

#endif // !_MATRIX_H_
