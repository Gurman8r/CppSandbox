#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <MemeCore/IEnumerable.h>
#include <MemeCore/IComparable.h>
#include <vector>
#include <initializer_list>
#include <algorithm>

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
		using data_type	= T;
		using self_type	= Matrix<data_type, _Cols, _Rows>;

		enum
		{
			Cols = _Cols,
			Rows = _Rows,
			Size = Cols * Rows 
		};

	private:
		data_type m_data[Size];

	public:
		Matrix()
			: IEnumerable<data_type>(&m_data[0], &m_data[Size])
		{
		}
		
		Matrix(const data_type & value)
			: self_type()
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value;
			}
		}
		
		Matrix(const data_type * value)
			: self_type()
		{
			for (std::size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}
		
		Matrix(const std::initializer_list<data_type> & value)
			: self_type()
		{
			for (auto it = value.begin(); it != value.end(); it++)
			{
				(*this)[(it - value.begin())] = (*it);
			}
		}
		
		Matrix(const self_type & value)
			: self_type(value.ptr())
		{
		}
		
		template <typename U, std::size_t C, std::size_t R>
		Matrix(const Matrix<U, C, R> & copy)
			: self_type()
		{
			const std::size_t imax = std::min<std::size_t>(Size, copy.Size);

			for (std::size_t i = 0; i < imax; i++)
			{
				(*this)[i] = static_cast<data_type>(copy[i]);
			}
		}

		virtual ~Matrix() {}

	public:
		inline const data_type * ptr() const
		{
			return m_data;
		}
		
		inline const data_type & operator[](std::size_t index) const
		{
			return m_data[index];
		}
		
		inline data_type & operator[](std::size_t index)
		{
			return m_data[index];
		}

		inline static self_type identity()
		{
			self_type value;
			for (std::size_t y = 0; y < Rows; y++)
			{
				for (std::size_t x = 0; x < Cols; x++)
				{
					value[y * Cols + x] = (x == y) ? (data_type)1 : (data_type)0;
				}
			}
			return value;
		}

	public:
		inline virtual void serialize(std::ostream & out) const override
		{
			out << "{ ";
			for (std::size_t y = 0; y < Rows; y++)
			{
				for (std::size_t x = 0; x < Cols; x++)
				{
					out << (*this)[y * Cols + x]
						<< ((x < Cols - 1) ? ", " : " }");
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
				if (in.good())
				{
					in >> (*this)[i];
				}
				else
				{
					(*this)[i] = data_type();
				}
			}
		}

	public:
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

	public:
		inline static const std::vector<T> & Flatten(const self_type * value, std::size_t length)
		{
			static std::vector<T> out;
			if (const std::size_t imax = (length * Size))
			{
				if (out.size() != imax)
				{
					out.resize(imax);
				}
				for (std::size_t i = 0; i < imax; i++)
				{
					out[i] = value[i / Size][i % Size];
				}
			}
			else if(!out.empty())
			{
				out.clear();
			}
			return out;
		}
	};

	template <typename T, std::size_t N> 
	using MatrixNxN = Matrix<T, N, N>;
	
	template <typename T> 
	using Matrix3 = MatrixNxN<T, 3>;
	
	template <typename T> 
	using Matrix4 = MatrixNxN<T, 4>;

	using mat3f = Matrix3<float>;
	using mat4f = Matrix4<float>;
}

#endif // !_MATRIX_H_
