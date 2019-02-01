#ifndef _BYTE_POOL_HPP_
#define _BYTE_POOL_HPP_

#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IEnumerable.hpp>

namespace ml
{
	template <typename T, size_t MaxBytes>
	class MemoryPool final
		: public ITrackable
		, public IEnumerable<T>
	{
	public:
		using size_type	= size_t;
		using data_type	= T;
		using pointer	= data_type * ;
		using reference	= data_type & ;
		using self_type	= MemoryPool<T, MaxBytes>;
		
	public:
		MemoryPool()
			: IEnumerable<T>(&m_data[0], &m_data[MaxBytes])
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
		inline const size_type	capacity()	const { return MaxBytes; }

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
			out << ml::endl;
		}

	private:
		data_type	m_data[MaxBytes];
		size_type	m_size;
	};

	template <size_t N>
	using BytePool = MemoryPool<uint8_t, N>;
}

#endif // !_BYTE_POOL_HPP_
