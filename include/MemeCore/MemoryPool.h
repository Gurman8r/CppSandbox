#ifndef _BYTE_POOL_H_
#define _BYTE_POOL_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/IEnumerable.h>

namespace ml
{
	template <typename T, size_t N>
	class MemoryPool final
		: public ITrackable
		, public IEnumerable<T>
	{
	public:
		using size_type	= size_t;
		enum : size_type { Size = N };

		using data_type			= T;
		using pointer			= data_type * ;
		using reference			= data_type & ;
		using self_type			= MemoryPool<data_type, Size>;

	public:
		MemoryPool()
			: IEnumerable<T>(&m_data[0], &m_data[Size])
			, m_index(0)
		{
		}
		~MemoryPool()
		{
		}

	public:
		inline bool initialize(T value)
		{
			return size() && memset(&m_data, value, size());
		}

		inline void * allocate(size_type value)
		{
			return ((size() && ((value + m_index) > size()))
				? (NULL)
				: (&m_data[m_index += value]));
		}

		template <typename U>
		inline U * allocate()
		{
			return static_cast<U*>(allocate(sizeof(U)));
		}

	public:
		inline const pointer	ptr()	const { return m_data; }
		inline const size_type	index()	const { return m_index; }
		inline const size_type	size()	const { return Size; }

	public:
		inline const reference operator[](size_type i) const { return m_data[i]; }
		inline reference	   operator[](size_type i)		 { return m_data[i]; }

	public:
		inline void serialize(std::ostream & out) const override
		{
			for (auto e : (*this))
			{
				out << e << " ";
			}
			out << std::endl;
		}

	private:
		data_type	m_data[Size];
		size_type	m_index;
	};

	template <size_t N>
	using BytePool = MemoryPool<uint8_t, N>;
}

#endif // !_BYTE_POOL_H_
