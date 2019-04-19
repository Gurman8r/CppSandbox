#ifndef _ML_RENDER_VAR_HPP_
#define _ML_RENDER_VAR_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RenderVar final
		: public ITrackable
		, public IHandle<int64_t>
	{
	public:
		RenderVar();
		RenderVar(const uint32_t type, const int64_t data);
		RenderVar(const RenderVar & copy);
		~RenderVar();

	public:
		inline const uint32_t & type() const { return m_type; }

	private:
		uint32_t m_type;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_VAR_HPP_