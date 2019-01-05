#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <MemeGraphics/Image.h>

namespace ml
{
	class ML_GRAPHICS_API Texture final
		: public ITrackable
		, public IResource
	{
	public:
		Texture();
		~Texture();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

	private:

	};
}

#endif // !_TEXTURE_H_
