#ifndef _ML_MANIFEST_ITEM_HPP_
#define _ML_MANIFEST_ITEM_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	struct ML_ENGINE_API ManifestItem final 
		: public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using Data = HashMap<String, String>;
		using iterator = typename Data::iterator;
		using const_iterator = typename Data::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * */

		Data data;

		/* * * * * * * * * * * * * * * * * * * * */

		ManifestItem();
		ManifestItem(const Data & data);
		ManifestItem(const ManifestItem & copy);

		/* * * * * * * * * * * * * * * * * * * * */

		inline const String & operator[](const String & key) const 
		{
			return data.at(key); 
		}

		inline String & operator[](const String & key) 
		{
			return data[key]; 
		}

		/* * * * * * * * * * * * * * * * * * * * */

		const String getStr(const String & value, const String & dv = String()) const;

		const int32_t getInt(const String & value, const int32_t dv = 0) const;

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

#endif // !_ML_MANIFEST_ITEM_HPP_