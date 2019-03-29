#ifndef _ML_BUILDER_HPP_
#define _ML_BUILDER_HPP_

#include <MemeEditor/GUI_Window.hpp>
#include <MemeEditor/Document.hpp>
#include <MemeGraphics/Shader.hpp>

#define ML_Builder ml::Builder::getInstance()
#define ML_MAX_SRC 1024

namespace ml
{
	// Shader Builder/Editor
	class ML_EDITOR_API Builder final
		: public GUI_Window
		, public ISingleton<Builder>
	{
		friend class ISingleton<Builder>;

	public:
		using SourceBuf = char[2048];

	private:
		Builder();
		~Builder();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;

	private:
		void draw_shader_tab(const String & label, SourceBuf & source);
		void draw_uniform_list(const String & value);
		void draw_uniform_data(Uniform * u);
		void draw_uniform_buttons(List<Uniform> & value);
		void draw_shader_source(CString label, SourceBuf & source);

	private:
		inline Uniform * get_uniform(List<Uniform> & u, const size_t i)
		{
			return (!u.empty()
				? (i < u.size())
					? (&u.at(i))
					: (NULL)
				: (NULL));
		}

		inline Uniform * get_uniform(const String & label, const size_t i)
		{
			return get_uniform(m_uni[label], i);
		}

		inline Uniform * get_selected(const String & label)
		{
			return get_uniform(label, m_selected);
		}

	private:
		size_t m_selected; // Selected Uniform

		HashMap<String, List<Uniform>>	m_uni; // Uniforms
		HashMap<String, SourceBuf>		m_src; // Sources

	};
}

#endif // !_ML_BUILDER_HPP_