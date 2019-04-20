#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IEnumerable.hpp>
#include <MemeCore/List.hpp>
#include <MemeCore/Maths.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	template <
		class	_Elem, 
		size_t	_Cols, 
		size_t	_Rows
	> class Matrix
		: public ITrackable
		, public IComparable<Matrix<_Elem, _Cols, _Rows>>
		, public IEnumerable<_Elem, _Cols * _Rows>
	{
	public: // Enums
		/* * * * * * * * * * * * * * * * * * * * */
		enum : size_t
		{
			Cols = _Cols,
			Rows = _Rows,
			Size = (Cols * Rows)
		};

	public: // Usings
		/* * * * * * * * * * * * * * * * * * * * */
		using value_type			= typename _Elem;
		using array_type			= typename value_type[Size];
		using pointer				= typename value_type *;
		using reference				= typename value_type &;
		using const_pointer			= typename const value_type *;
		using const_reference		= typename const value_type &;

		using self_type				= typename Matrix<value_type, Cols, Rows>;
		using init_type				= typename std::initializer_list<value_type>;
		using contiguous_type		= typename List<value_type>;

		using enumerable_type		= typename IEnumerable<value_type, Size>;
		using iterator				= typename enumerable_type::iterator;
		using const_iterator		= typename enumerable_type::const_iterator;
		using reverse_iterator		= typename enumerable_type::reverse_iterator;
		using const_reverse_iterator= typename enumerable_type::const_reverse_iterator;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
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
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = value[i];
			}
		}

		Matrix(const init_type & value)
			: self_type()
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = (((value.begin() + i) < value.end())
					? (*(value.begin() + i))
					: (static_cast<value_type>(0))
				);
			}
		}

		Matrix(const self_type & value)
			: self_type(value.m_data)
		{
		}

		template <
			class	E,
			size_t	C,
			size_t	R
		> Matrix(const Matrix<E, C, R> & value, const_reference dv = static_cast<value_type>(0))
			: self_type()
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = ((i < value.size())
					? (static_cast<value_type>(value[i]))
					: (dv)
				);
			}
		}

		virtual ~Matrix() {}


	public: // Member Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline constexpr size_t	cols()	const noexcept		{ return self_type::Cols;	}
		inline constexpr size_t	rows()	const noexcept		{ return self_type::Rows;	}
		inline constexpr size_t	size()	const noexcept		{ return self_type::Size;	}
		
		inline reference		at(const size_t i)			{ return (*this)[i];		}
		inline reference		back()						{ return at(size() - 1);	}
		inline pointer			data()						{ return &at(0);			}
		inline reference		front()						{ return at(0);				}

		inline const_reference	at(const size_t i)	const	{ return (*this)[i];		}
		inline const_reference	back()				const	{ return at(size() - 1);	}
		inline const_pointer	data()				const	{ return &at(0);			}
		inline const_reference	front()				const	{ return at(0);				}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class Iter
		> inline self_type & copy(Iter first, Iter last)
		{
			std::copy(first, last, this->begin());
			return (*this);
		}

		inline self_type & copy(const self_type & value)
		{
			return this->copy(value.begin(), value.end());
		}

		inline self_type & fill(const_reference value)
		{
			std::fill(this->begin(), this->end(), value);
			return (*this);
		}

		inline self_type & swap(self_type & other)
		{
			std::swap(this->data(), other.data());
			return other;
		}


	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline static const contiguous_type & Contiguous(const self_type * value, size_t length)
		{
			static contiguous_type out;
			{
				if (const size_t imax = (length * self_type::Size))
				{
					if (out.size() != imax)
					{
						out.resize(imax);
					}
					for (size_t i = 0; i < imax; i++)
					{
						out[i] = value[i / self_type::Size][i % self_type::Size];
					}
				}
				else if (!out.empty())
				{
					out.clear();
				}
			}
			return out;
		}

		inline static const self_type & Identity()
		{
			static self_type temp;
			static bool check = true;
			if (check)
			{	check = false;
				for (size_t y = 0; y < self_type::Rows; y++)
				{
					for (size_t x = 0; x < self_type::Cols; x++)
					{
						temp[y * self_type::Cols + x] = ((x == y)
							? static_cast<value_type>(1)
							: static_cast<value_type>(0)
						);
					}
				}
			}
			return temp;
		}


	public: // Overrides
		/* * * * * * * * * * * * * * * * * * * * */
		inline virtual void serialize(std::ostream & out) const override
		{
			for (size_t y = 0; y < this->rows(); y++)
			{
				for (size_t x = 0; x < this->cols(); x++)
				{
					out << (*this)[y * this->cols() + x] << " ";
				}
				out << ml::endl;
			}
			out << ml::endl;
		}
		
		inline virtual void deserialize(std::istream & in) override
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				if (in.good())
				{
					in >> (*this)[i];
				}
				else
				{
					(*this)[i] = static_cast<value_type>(0);
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline virtual bool equals(const self_type & value) const override
		{
			for (size_t i = 0; i < this->size(); i++)
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
			for (size_t i = 0; i < this->size(); i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline self_type & operator=(const_reference value)
		{
			return ((*this) = self_type(value));
		}

		inline const_reference operator[](size_t index) const
		{
			assert((index < this->size()) && "Matrix subscript out of range!");
			return m_data[index];
		}

		inline reference operator[](size_t index)
		{
			assert((index < this->size()) && "Matrix subscript out of range!");
			return m_data[index];
		}


	private: // Data
		/* * * * * * * * * * * * * * * * * * * * */
		array_type m_data;
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Constants
	/* * * * * * * * * * * * * * * * * * * * */
	template <class T, size_t N> using MatrixNxN = Matrix<T, N, N>;
	template <class T>			 using Matrix3x3 = MatrixNxN<T, 3>;
	template <class T>			 using Matrix4x4 = MatrixNxN<T, 4>;

	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using mat3f = Matrix3x3<float>;
	using mat4f = Matrix4x4<float>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_MATRIX_HPP_