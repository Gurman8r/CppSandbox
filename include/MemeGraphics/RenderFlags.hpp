#ifndef _ML_RENDER_FLAGS_HPP_
#define _ML_RENDER_FLAGS_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_GRAPHICS_API RenderFlags final
		: public ITrackable
	{
	public:
		using Initializer		= typename std::initializer_list<Pair<uint32_t, int32_t>>;
		using FlagMap			= typename HashMap<uint32_t, int32_t>;
		using iterator			= typename FlagMap::iterator;
		using const_iterator	= typename FlagMap::const_iterator;

	public:
		RenderFlags();
		RenderFlags(const Initializer & data);
		RenderFlags(const HashMap<uint32_t, int32_t> & data);
		RenderFlags(const RenderFlags & copy);
		~RenderFlags();

	public:
		void apply() const;

	private:
		bool applyFlag(uint32_t key, int32_t value) const;

	public:
		inline int32_t operator[](uint32_t key) const
		{
			return m_data.at(key);
		}

		inline int32_t & operator[](uint32_t key)
		{
			return m_data[key];
		}
		
	public:
		inline iterator			begin()			{ return m_data.begin();  }
		inline const_iterator	begin()	const	{ return m_data.begin();  }
		inline const_iterator	cbegin()const	{ return m_data.cbegin(); }
		inline iterator			end()			{ return m_data.end();	  }
		inline const_iterator	end()	const	{ return m_data.end();	  }
		inline const_iterator	cend()	const	{ return m_data.cend();	  }

	private:
		HashMap<uint32_t, int32_t> m_data;
	};
}

#endif // !_ML_RENDER_FLAGS_HPP_