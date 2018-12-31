#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <MemeLib/IEnumerable.h>
#include <MemeLib/Maths.h>
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
		using enum_type		= IEnumerable<T>;
		using self_type		= Matrix<T, _Cols, _Rows>;

		static const std::size_t Cols = _Cols;
		static const std::size_t Rows = _Rows;
		static const std::size_t Size = (Rows * Cols);

	private:
		T m_data[Size];

	public:
		Matrix()
			: enum_type(&m_data[0], &m_data[Size])
		{
		}
		Matrix(const T & value)
			: self_type()
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value;
			}
		}
		Matrix(const T * value)
			: self_type()
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}
		Matrix(const std::initializer_list<T> & value)
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

		inline static self_type identity()
		{
			self_type value;
			for (std::size_t y = 0; y < Rows; y++)
			{
				for (std::size_t x = 0; x < Cols; x++)
				{
					value[y * Cols + x] = (x == y) ? (T)1 : (T)0;
				}
			}
			return value;
		}

		template <typename U, std::size_t C, std::size_t R>
		inline self_type & copyData(const Matrix<U, C, R> & copy)
		{
			for (std::size_t i = 0, imax = std::min((*this).Size, copy.Size); i < imax; i++)
			{
				(*this)[i] = static_cast<T>(copy[i]);
			}
			return (*this);
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

	template <typename T> using Matrix3 = Matrix<T, 3, 3>;
	template <typename T> using Matrix4 = Matrix<T, 4, 4>;

	using Matrix3f = Matrix3<float>;
	using Matrix4f = Matrix4<float>;

	using mat3f = Matrix3f;
	using mat4f = Matrix4f;
}

#endif // !_MATRIX_H_
