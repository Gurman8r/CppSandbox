#ifndef _BYTE_POOL_H_
#define _BYTE_POOL_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/IEnumerable.h>

namespace ml
{
	template <typename T, size_t Capacity>
	class MemoryPool final
		: public ITrackable
		, public IEnumerable<T>
	{
	public:
		using size_type	= size_t;
		using data_type	= T;
		using pointer	= data_type * ;
		using reference	= data_type & ;
		using self_type	= MemoryPool<T, Capacity>;
		
	public:
		MemoryPool()
			: IEnumerable<T>(&m_data[0], &m_data[Capacity])
			, m_size(0)
		{
		}
		~MemoryPool()
		{
		}

	public:
		inline bool initialize(T value)
		{
			return capacity() && memset(&m_data, value, capacity());
		}

		inline void * allocate(size_type value)
		{
			return (value && ((value + m_size) > capacity()))
				? NULL
				: (&m_data[m_size += value]);
		}

	public:
		inline const size_type	index()		const { return m_size; }
		inline const size_type	capacity()	const { return Capacity; }

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
		data_type	m_data[Capacity];
		size_type	m_size;
	};

	template <size_t N>
	using BytePool = MemoryPool<uint8_t, N>;
}

#endif // !_BYTE_POOL_H_
