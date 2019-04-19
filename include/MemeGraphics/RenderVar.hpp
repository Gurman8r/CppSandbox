#ifndef _ML_RENDER_VAR_HPP_
#define _ML_RENDER_VAR_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	// FIXME: needs some work
	class ML_GRAPHICS_API RenderVar final
		: public ITrackable
	{
	public:
		RenderVar();
		RenderVar(const uint32_t type, void * data);
		RenderVar(const RenderVar & copy);
		~RenderVar();

		template <
			class T
		> RenderVar(const uint32_t type, T && data)
			: RenderVar(type, (void *)data)
		{
		}

	public:
		inline const uint32_t & type() const { return m_type; }

		inline const void * data() const { return m_data; }

		inline void * & data() { return m_data; }

	private:
		uint32_t m_type;
		void * m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_VAR_HPP_