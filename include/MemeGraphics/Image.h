#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <MemeGraphics/Color.h>
#include <MemeCore/IResource.h>

namespace ml
{
	class ML_GRAPHICS_API Image final
		: public ITrackable
		, public IResource
	{
	public:
		Image();
		~Image();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

	private:

	};
}

#endif // !_IMAGE_H_
