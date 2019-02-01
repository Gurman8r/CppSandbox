#ifndef _ML_TYPE_INFO_HPP_
#define _ML_TYPE_INFO_HPP_

#include <MemeCore/Export.hpp>
#include <MemeCore/String.hpp>

namespace ml
{
	class ML_CORE_API TypeInfo final
	{	
	public:

		TypeInfo()
			: m_name(0)
			, m_raw	(0)
			, m_hash(0)
		{
		}
		TypeInfo(const std::type_info & value)
			: m_name(value.name())
			, m_raw	(value.raw_name())
			, m_hash(value.hash_code())
		{
		}
		TypeInfo(const TypeInfo & copy)
			: m_name(copy.m_name)
			, m_raw	(copy.m_raw)
			, m_hash(copy.m_hash)
		{
		}
		~TypeInfo() { }

		inline const char * name() const { return m_name; }
		inline const char * raw_name() const { return m_raw; }
		inline const size_t hash_code() const { return m_hash; }

		inline friend std::ostream & operator<<(std::ostream & out, const TypeInfo & value)
		{
			return out << value.name();
		}

	private:
		const char *	m_name;
		const char *	m_raw;
		size_t			m_hash;
	};
}

#endif // !_ML_TYPE_INFO_HPP_
