#ifndef _SHADER_PARSER_HPP_
#define _SHADER_PARSER_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeGraphics/Shader.hpp>

namespace ml
{
	class ML_EDITOR_API ShaderParser final
		: public ITrackable
	{
	public:
		ShaderParser();
		~ShaderParser();

		bool parseShader(const String & text);

	private:

	};

	
}

#endif // !_SHADER_PARSER_HPP_
