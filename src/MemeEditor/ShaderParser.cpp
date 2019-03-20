#include <MemeEditor/ShaderParser.hpp>

namespace ml
{
	ShaderParser::ShaderParser()
	{
	}

	ShaderParser::~ShaderParser()
	{
	}
	bool ShaderParser::parseShader(const String & source)
	{
		enum { NONE = -1, VERT, FRAG, GEOM, MAX };

		int8_t	srcType = NONE;
		SStream	srcData[MAX];

		SStream	stream(source);
		String	line;
		while (std::getline(stream, line))
		{
			if (line.find("#shader") != String::npos)
			{
				if (line.find("vertex") != String::npos)
				{
					srcType = VERT;
				}
				else if (line.find("fragment") != String::npos)
				{
					srcType = FRAG;
				}
				else if (line.find("geometry") != String::npos)
				{
					srcType = GEOM;
				}
			}
			else if (srcType > NONE)
			{
				srcData[srcType] << line << std::endl;
			}
		}

		return false;
	}
}