#ifndef _SHADER_BUILDER_HPP_
#define _SHADER_BUILDER_HPP_

#include <MemeEditor/SourceFile.hpp>
#include <MemeGraphics/Shader.hpp>

#define ML_Builder ml::Builder::getInstance()
#define ML_MAX_SRC 1024

namespace ml
{
	// Shader Builder/Editor
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
		void draw_shader_tab(const String & label, List<char> & source);

		void draw_uniform_edit(Uniform * u);
		void draw_uniform_list(const String & value);
		void draw_uniform_list_buttons(List<Uniform> & value);
		void draw_source_tab(CString label, List<char> & source);
		
	public:
		int32_t textEditCallback();

	private:
		inline Uniform * get_uniform(List<Uniform> & u, const size_t i)
		{
			return (!u.empty()
				? (i < u.size())
					? &u.at(i)
					: NULL
				: NULL);
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
		bool *	m_open		= 0;
		size_t	m_selected	= 0;
		char	m_inputBuf	[64];

		HashMap<String, List<char>>		m_s; // Sources
		HashMap<String, List<Uniform>>	m_u; // Uniforms

	};
}

#endif // !_SHADER_BUILDER_HPP_
