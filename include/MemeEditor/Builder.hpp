#ifndef _ML_BUILDER_HPP_
#define _ML_BUILDER_HPP_

#include <MemeEditor/Editor.hpp>
#include <MemeEditor/Document.hpp>
#include <MemeGraphics/Shader.hpp>

#define ML_Builder ml::Builder::getInstance()
#define ML_MAX_SRC 1024

namespace ml
{
	// Shader Builder/Editor
	class ML_EDITOR_API Builder
		: public Editor
		, public ISingleton<Builder>
	{
		friend class ISingleton<Builder>;

	public:
		using SourceBuf = char[2048];

	private:
		Builder();
		~Builder();

	public:
		bool draw(CString title, bool * p_open) override;

	private:
		void draw_shader_tab(const String & label, SourceBuf & source);
		void draw_uniform_data(Uniform * u);
		void draw_uniform_list(const String & value);
		void draw_uniform_list_buttons(List<Uniform> & value);
		void draw_source_tab(CString label, SourceBuf & source);

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
			return get_uniform(m_u[label], i);
		}

		inline Uniform * get_selected(const String & label)
		{
			return get_uniform(label, m_selected);
		}

	private:
		bool *	m_open;
		size_t	m_selected;
		char	m_inputBuf[64];

		HashMap<String, SourceBuf>		m_s; // Sources
		HashMap<String, List<Uniform>>	m_u; // Uniforms

	};
}

#endif // !_ML_BUILDER_HPP_