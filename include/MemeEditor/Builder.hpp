#ifndef _SHADER_BUILDER_HPP_
#define _SHADER_BUILDER_HPP_

#include <MemeEditor/Editor.hpp>
#include <MemeGraphics/Shader.hpp>

#define ML_Builder ml::Builder::getInstance()

namespace ml
{
	class ML_EDITOR_API Builder
		: public ITrackable
		, public ISingleton<Builder>
	{
		friend class ISingleton<Builder>;

	private:
		Builder();
		~Builder();

	public:
		void draw(bool * p_open);

	private:

		void draw_menu();
		void draw_list();
		void draw_preview();
		void draw_uniform(Uniform & uniform);

		inline Uniform * get_uniform(const size_t i)
		{
			return (!m_uniforms.empty()
				? (i < m_uniforms.size())
					? &m_uniforms.at(i)
					: NULL
				: NULL);
		}

		inline Uniform * get_selected()
		{
			return get_uniform(m_selected);
		}

	private:
		bool *			m_open;
		String			m_source = "Placeholder Source";
		List<Uniform>	m_uniforms;
		size_t			m_selected = 0;
	};
}

#endif // !_SHADER_BUILDER_HPP_
