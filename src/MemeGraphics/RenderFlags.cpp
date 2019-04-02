#include <MemeGraphics/RenderFlags.hpp>
#include <MemeGraphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	RenderFlags::RenderFlags()
		: m_data()
	{
	}

	RenderFlags::RenderFlags(const HashMap<uint32_t, int32_t>& data)
		: m_data(data)
	{
	}

	RenderFlags::RenderFlags(const RenderFlags & copy)
		: m_data(copy.m_data)
	{
	}

	RenderFlags::~RenderFlags()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void RenderFlags::apply() const
	{
		for (auto pair : (*this))
		{
			if (!applyFlag(pair.first, pair.second))
			{
				// Something went wrong...
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool RenderFlags::applyFlag(uint32_t key, int32_t value) const
	{
		switch (value)
		{
		case 0:
			return ML_GL.disable(key);
		case 1:
			return ML_GL.enable(key);
		default:
			return false;
		}
	}


	/* * * * * * * * * * * * * * * * * * * * */
}