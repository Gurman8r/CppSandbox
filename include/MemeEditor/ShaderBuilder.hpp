#ifndef _SHADER_BUILDER_HPP_
#define _SHADER_BUILDER_HPP_

#include <MemeEditor/Editor.hpp>
#include <MemeGraphics/Shader.hpp>

#define ML_ShaderBuilder ml::ShaderBuilder::getInstance()

namespace ml
{
	class ML_EDITOR_API ShaderBuilder
		: public ITrackable
		, public ISingleton<ShaderBuilder>
	{
		friend class ISingleton<ShaderBuilder>;

	public:
		void draw(CString title, bool * p_open);

		void editSource(String & source);
		void editUniforms(List<Uniform> & values);
		void editUniformList(List<Uniform> & values);
		void editUniformValue(Uniform & uniform);

	private:
		ShaderBuilder();
		~ShaderBuilder();

		String m_source;

		List<Uniform> m_uniforms;
		size_t m_selected = 0;
	};
}

#endif // !_SHADER_BUILDER_HPP_
