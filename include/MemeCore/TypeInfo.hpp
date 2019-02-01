#ifndef _ML_TYPE_INFO_H_
#define _ML_TYPE_INFO_H_

#include <MemeCore/Export.hpp>
#include <MemeCore/String.hpp>

namespace ml
{
	struct ML_CORE_API TypeInfo final
	{
		const char *	name;
		const char *	rawName;
		size_t			hashCode;

		TypeInfo()
			: name		(0)
			, rawName	(0)
			, hashCode	(0)
		{
		}
		TypeInfo(const std::type_info & value)
			: name		(value.name())
			, rawName	(value.raw_name())
			, hashCode	(value.hash_code())
		{
		}
		TypeInfo(const TypeInfo & copy)
			: name		(copy.name)
			, rawName	(copy.rawName)
			, hashCode	(copy.hashCode)
		{
		}
		~TypeInfo() { }
	};
}

#endif // !_ML_TYPE_INFO_H_
