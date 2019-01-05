#ifndef _SHADER_H_
#define _SHADER_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/IResource.h>

namespace ml
{
	class ML_GRAPHICS_API Shader
		: public ITrackable
		, public IResource
	{
	public:
		Shader();
		~Shader();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

	private:

	};
}

#endif // !_SHADER_H_
