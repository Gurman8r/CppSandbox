#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <MemeCore/IEnumerable.hpp>
#include <MemeCore/List.hpp>
#include <MemeCore/Maths.hpp>

namespace ml
{
	// Template Fixed 2D Array
	template <class _Elem, size_t _Cols, size_t _Rows>
	class Matrix
		: public ITrackable
		, public IComparable<Matrix<_Elem, _Cols, _Rows>>
		, public IEnumerable<_Elem, _Cols * _Rows>
	{
	public:
		enum : size_t
		{
			Cols = _Cols,
			Rows = _Rows,
			Size = Cols * Rows
		};
		
		using value_type			= _Elem;
		using self_type				= Matrix<value_type, Cols, Rows>;
		using enumerable_type		= IEnumerable<value_type, Size>;

		using initializer_type		= typename std::initializer_list<value_type>;
		using array_type			= typename std::array<value_type, Size>;
		using contiguous_type		= typename std::vector<value_type>;

		using pointer				= typename value_type *;
		using const_pointer			= typename const value_type *;
		using reference				= typename value_type &;
		using const_reference		= typename const value_type &;

		using iterator				= typename enumerable_type::iterator;
		using const_iterator		= typename enumerable_type::const_iterator;
		using reverse_iterator		= typename enumerable_type::reverse_iterator;
		using const_reverse_iterator= typename enumerable_type::const_reverse_iterator;

	private:
		value_type m_data[Size];

	public:
		Matrix()
			: enumerable_type(m_data)
		{
		}

		Matrix(const_reference value)
			: self_type()
		{
			this->fill(value);
		}

		Matrix(const_pointer value)
			: self_type()
		{
			for (size_t i = 0; i < Size; i++)
			{
				(*this)[i] = value[i];
			}
		}

		Matrix(const initializer_type & value)
			: self_type()
		{
			if (value.size() == Size)
			{
				std::copy(value.begin(), value.end(), this->begin());
			}
			else
			{
				for (size_t i = 0; i < Size; i++)
				{
					auto it = (value.begin() + i);

					(*this)[i] = (it < value.end()) ? (*it) : (value_type)(0);
				}
			}
		}

		Matrix(const self_type & value)
			: self_type(value.data())
		{
		}

		template <class U, size_t C, size_t R>
		Matrix(const Matrix<U, C, R> & value, const_reference dv = (value_type)(0))
			: self_type()
		{
			for (size_t i = 0; i < Size; i++)
			{
				(*this)[i] = ((i < value.Size)
					? static_cast<value_type>(value[i])
					: dv);
			}
		}

		virtual ~Matrix() {}


	public:
		inline void fill(const_reference value)
		{
			std::fill(this->begin(), this->end(), value);
		}

		inline void swap(self_type & other)
		{
			std::swap(m_data, other.m_data);
		}

	public:
		inline self_type & operator=(const_reference value)
		{
			return ((*this) = self_type(value));
		}
		
		inline const_reference operator[](size_t index) const
		{
			assert((index < Size) && "Matrix subscript out of range!");
			return m_data[index];
		}
		
		inline reference operator[](size_t index)
		{
			assert((index < Size) && "Matrix subscript out of range!");
			return m_data[index];
		}
		
	public:
		inline pointer		data()
		{
			return m_data;
		}
		inline reference	at(size_t i)
		{
			return (*this)[i];
		}
		inline reference	at(size_t x, size_t y)
		{
			return at(y * Cols + x);
		}
		inline reference	front()
		{
			return at(0);
		}
		inline reference	back()
		{
			return at(size() - 1);
		}

		inline const_pointer	data() const
		{
			return m_data;
		}
		inline const_reference	at(size_t i) const
		{
			return (*this)[i];
		}
		inline const_reference	at(size_t x, size_t y) const
		{
			return at(y * Cols + x);
		}
		inline const_reference	front() const
		{
			return at(0);
		}
		inline const_reference	back() const
		{
			return at(size() - 1);
		}

		inline size_t cols() const { return Cols; }
		inline size_t rows() const { return Rows; }
		inline size_t size() const { return Size; }

	public:
		inline virtual void serialize(std::ostream & out) const override
		{
			for (size_t y = 0; y < Rows; y++)
			{
				for (size_t x = 0; x < Cols; x++)
				{
					out << (*this)[y * Cols + x] << ' ';
				}
				out << ml::endl;
			}
			out << ml::endl;
		}
		
		inline virtual void deserialize(std::istream & in) override
		{
			for (size_t i = 0; i < Size; i++)
			{
				if (in.good())
				{
					in >> (*this)[i];
				}
				else
				{
					(*this)[i] = (value_type)0;
				}
			}
		}

	public:
		inline virtual bool equals(const self_type & value) const override
		{
			for (size_t i = 0; i < Size; i++)
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
			for (size_t i = 0; i < Size; i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}

	public:
		inline static self_type & identity()
		{
			static self_type temp;
			static bool checked = false;
			if (!checked)
			{	
				checked = true;
				for (size_t y = 0; y < Rows; y++)
				{
					for (size_t x = 0; x < Cols; x++)
					{
						temp.at(x, y) = (x == y) ? (value_type)1 : (value_type)0;
					}
				}
			}
			return temp;
		}

		inline static const contiguous_type & contiguous(const self_type * value, size_t length)
		{
			static contiguous_type out;
			if (const size_t imax = (length * Size))
			{
				if (out.size() != imax)
				{
					out.resize(imax);
				}
				for (size_t i = 0; i < imax; i++)
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

	template <class _Elem, size_t N> 
	using MatrixN = Matrix<_Elem, N, N>;
	
	template <class _Elem> 
	using Matrix3 = MatrixN<_Elem, 3>;
	
	template <class _Elem> 
	using Matrix4 = MatrixN<_Elem, 4>;

	using mat3f = Matrix3<float>;
	using mat4f = Matrix4<float>;
}

#endif // !_ML_MATRIX_HPP_