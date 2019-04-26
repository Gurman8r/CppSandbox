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
		static_assert(std::is_trivial<_Elem>::value, 
			"Matricies must contain trivial values."
		);


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
		using init_type				= typename Initializer<value_type>;
		using contiguous_type		= typename List<value_type>;

		using enumerable_type		= typename IEnumerable<value_type, Size>;
		using iterator				= typename enumerable_type::iterator;
		using const_iterator		= typename enumerable_type::const_iterator;
		using reverse_iterator		= typename enumerable_type::reverse_iterator;
		using const_reverse_iterator= typename enumerable_type::const_reverse_iterator;


	private: // Data
		/* * * * * * * * * * * * * * * * * * * * */
		array_type m_data;


	public: // Constructors
		/* * * * * * * * * * * * * * * * * * * * */
		Matrix()
			: enumerable_type(m_data)
			, m_data()
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = static_cast<value_type>(0);
			}
		}

		Matrix(const_reference value)
			: enumerable_type(m_data)
			, m_data()
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = value;
			}
		}

		Matrix(const_pointer value)
			: enumerable_type(m_data)
			, m_data()
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = value[i];
			}
		}

		Matrix(const init_type & value)
			: enumerable_type(m_data)
			, m_data()
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
			: enumerable_type(m_data)
			, m_data()
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = value[i];
			}
		}

		template <
			class	T,
			size_t	X,
			size_t	Y
		> Matrix(const Matrix<T, X, Y> & value, const_reference def = static_cast<value_type>(0))
			: enumerable_type(m_data)
			, m_data()
		{
			for (size_t i = 0; i < this->size(); i++)
			{
				(*this)[i] = ((i < value.size())
					? (static_cast<value_type>(value[i]))
					: (def)
				);
			}
		}

		virtual ~Matrix() {}


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
		inline const_reference operator[](const size_t index) const
		{
			assert((index < this->size()) && "Matrix subscript out of range!");
			return m_data[index];
		}

		inline reference operator[](const size_t index)
		{
			assert((index < this->size()) && "Matrix subscript out of range!");
			return m_data[index];
		}

		inline self_type & operator=(const_reference value)
		{
			return ((*this) = self_type(value));
		}


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


	public: // Static Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline static const contiguous_type & Contiguous(const self_type * value, const size_t length)
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
				out << endl;
			}
			out << endl;
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

# ifdef GLM_VERSION
	public: // GLM
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// From glm::mat3
		template <
			size_t X = self_type::Cols,
			size_t Y = self_type::Rows
		> Matrix(
			const glm::tmat3x3<value_type, glm::defaultp> & value,
			typename std::enable_if<(X == 3 && Y == 3)>::type * = 0)
			: self_type((const_pointer)(glm::value_ptr(value)))
		{
		}

		// To glm::mat3
		template <
			size_t X = self_type::Cols,
			size_t Y = self_type::Rows
		> inline operator glm::tmat3x3<value_type, glm::defaultp>() const
		{
			static_assert((X == 3 && Y == 3),
				"Size mismatch, unable to convert matrix to glm::mat3"
			);
			return glm::tmat3x3<value_type, glm::defaultp>(
				(*this)[0], (*this)[1], (*this)[2],
				(*this)[3], (*this)[4], (*this)[5],
				(*this)[6], (*this)[7], (*this)[8]
			);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		// From glm::mat4
		template <
			size_t X = self_type::Cols,
			size_t Y = self_type::Rows
		> Matrix(
			const glm::tmat4x4<value_type, glm::defaultp> & value,
			typename std::enable_if<(X == 4 && Y == 4)>::type * = 0)
			: self_type((const_pointer)(glm::value_ptr(value)))
		{
		}

		// To glm::mat4
		template <
			size_t X = self_type::Cols,
			size_t Y = self_type::Rows
		> inline operator glm::tmat4x4<value_type, glm::defaultp>() const
		{
			static_assert((X == 4 && Y == 4),
				"Size mismatch, unable to convert matrix to glm::mat4"
			);
			return glm::tmat4x4<value_type, glm::defaultp>(
				(*this)[ 0], (*this)[ 1], (*this)[ 2], (*this)[ 3],
				(*this)[ 4], (*this)[ 5], (*this)[ 6], (*this)[ 7],
				(*this)[ 8], (*this)[ 9], (*this)[10], (*this)[11],
				(*this)[12], (*this)[13], (*this)[14], (*this)[15]
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# endif
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	// Types
	/* * * * * * * * * * * * * * * * * * * * */
	using mat3f = Matrix<float, 3, 3>;
	using mat4f = Matrix<float, 4, 4>;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_MATRIX_HPP_