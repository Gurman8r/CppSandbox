#ifndef _SHADER_PARSER_HPP_
#define _SHADER_PARSER_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/File.hpp>

namespace ml
{
	class ML_GRAPHICS_API ShaderParser final
	{
	public:
		static bool parseShader(const String & src, SStream & v, SStream & g, SStream & f);

		static String parseIncludes(const String & src);

		static bool parseWrapped(const String & src, const char lhs, const char rhs, String & out);
	};
}

#endif // !_SHADER_PARSER_HPP_
