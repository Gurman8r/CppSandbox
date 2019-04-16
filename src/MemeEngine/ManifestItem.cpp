#include <MemeEngine/ManifestItem.hpp>
#include <MemeCore/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ManifestItem::ManifestItem()
		: data()
	{
	}

	ManifestItem::ManifestItem(const Data & data)
		: data(data)
	{
	}

	ManifestItem::ManifestItem(const ManifestItem & copy)
		: data(copy.data)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const String ManifestItem::getStr(const String & value, const String & dv) const
	{
		HashMap<String, String>::const_iterator it;
		return (((it = data.find(value)) != data.end()) ? it->second : dv);
	}

	const int32_t ManifestItem::getInt(const String & value, const int32_t dv) const
	{
		if (const String str = getStr(value))
		{
			int32_t temp;
			if (StringUtility::MakeInt(str, temp))
			{
				return temp;
			}
		}
		return dv;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}