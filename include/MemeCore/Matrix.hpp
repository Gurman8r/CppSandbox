#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <MemeCore/IEnumerable.hpp>
#include <MemeCore/List.hpp>
#include <MemeCore/Maths.hpp>

namespace ml
{
	// Template Fixed 2D Array
	template <class T, size_t _Cols, size_t _Rows>
	class Matrix
		: public ITrackable
		, public IComparable<Matrix<T, _Cols, _Rows>>
		, public IEnumerable<T>
	{
	public:
		using data_type	= T;
		using self_type	= Matrix<data_type, _Cols, _Rows>;
		using size_type = size_t;

		enum : size_type
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
			for (size_type i = 0; i < Size; i++)
			{
				(*this)[i] = value;
			}
		}
		
		Matrix(const data_type * value)
			: self_type()
		{
			for (size_type i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}
		
		Matrix(const std::initializer_list<data_type> & value)
			: self_type()
		{
			for (auto it = value.begin(); it != value.end(); it++)
			{
				size_type i;
				if ((i = (it - value.begin())) < Size)
				{
					(*this)[i] = (*it);
					continue;
				}
				break;
			}
		}
		
		Matrix(const self_type & value)
			: self_type(value.ptr())
		{
		}
		
		template <class U, size_type C, size_type R>
		Matrix(const Matrix<U, C, R> & copy, const data_type dv = (data_type)(0))
			: self_type()
		{
			for (size_type i = 0; i < Size; i++)
			{
				(*this)[i] = ((i < copy.Size)
					? static_cast<data_type>(copy[i])
					: dv);
			}
		}

		virtual ~Matrix() {}


	public:
		inline self_type & operator=(const data_type & value)
		{
			return (*this) = self_type(value);
		}

	public:
		inline const data_type * ptr() const
		{
			return m_data;
		}
		
		inline const data_type & operator[](size_type index) const
		{
			assert(index < Size && "Matrix subscript out of range!");
			return m_data[index];
		}
		
		inline data_type & operator[](size_type index)
		{
			assert(index < Size && "Matrix subscript out of range!");
			return m_data[index];
		}

		inline static self_type & identity()
		{
			static self_type temp;
			static bool check = false;
			if (!check)
			{
				check = true;
				for (size_type y = 0; y < Rows; y++)
				{
					for (size_type x = 0; x < Cols; x++)
					{
						temp[y * Cols + x] = (x == y) ? (data_type)1 : (data_type)0;
					}
				}
			}
			return temp;
		}

	public:
		inline virtual void serialize(std::ostream & out) const override
		{
			for (size_type y = 0; y < Rows; y++)
			{
				for (size_type x = 0; x < Cols; x++)
				{
					out << (*this)[y * Cols + x] << " ";
				}
				out << ml::endl;
			}
			out << ml::endl;
		}
		
		inline virtual void deserialize(std::istream & in) override
		{
			for (size_type i = 0; i < Size; i++)
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
			for (size_type i = 0; i < Size; i++)
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
			for (size_type i = 0; i < Size; i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}

	public:
		inline static const std::vector<T> & contiguous(const self_type * value, size_type length)
		{
			static std::vector<T> out;
			if (const size_type imax = (length * Size))
			{
				if (out.size() != imax)
				{
					out.resize(imax);
				}
				for (size_type i = 0; i < imax; i++)
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

	template <class T, size_t N> 
	using MatrixNxN = Matrix<T, N, N>;
	
	template <class T> 
	using Matrix3 = MatrixNxN<T, 3>;
	
	template <class T> 
	using Matrix4 = MatrixNxN<T, 4>;

	using mat3f = Matrix3<float>;
	using mat4f = Matrix4<float>;
}

#endif // !_MATRIX_HPP_
