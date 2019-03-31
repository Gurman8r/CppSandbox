#ifndef _ML_RESOURCE_ITEM_HPP_
#define _ML_RESOURCE_ITEM_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeScript/StringUtility.hpp>

namespace ml
{
	struct ManifestItem final : public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using Data = HashMap<String, String>;
		using iterator = typename Data::iterator;
		using const_iterator = typename Data::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * */

		Data data;

		/* * * * * * * * * * * * * * * * * * * * */

		ManifestItem() : data() {}
		ManifestItem(const Data & data) : data(data) {}
		ManifestItem(const ManifestItem & copy) : data(copy.data) {}

		/* * * * * * * * * * * * * * * * * * * * */

		inline const String & operator[](const String & key) const { return data.at(key); }

		inline String & operator[](const String & key) { return data[key]; }

		/* * * * * * * * * * * * * * * * * * * * */

		inline const String getStr(const String & value, const String & dv = String()) const
		{
			HashMap<String, String>::const_iterator it;
			return (((it = data.find(value)) != data.end()) ? it->second : dv);
		}

		inline const int32_t getInt(const String & value, const int32_t dv = 0) const
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

		inline iterator			begin()			{ return data.begin();  }
		inline iterator			end()			{ return data.end();	}
		inline const_iterator	begin()  const	{ return data.begin();  }
		inline const_iterator	end()	 const	{ return data.end();	}
		inline const_iterator	cbegin() const	{ return data.cbegin(); }
		inline const_iterator	cend()	 const	{ return data.cend();	}

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RESOURCE_ITEM_HPP_