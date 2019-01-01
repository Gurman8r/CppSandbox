#ifndef _VIDEO_MODE_H_
#define _VIDEO_MODE_H_

#include <MemeWindow/Export.h>
#include <MemeCore/Vector2.h>
#include <vector>

namespace ml
{
	struct ML_WINDOW_API VideoMode final
		: public ITrackable
		, public IComparable<VideoMode>
	{
		VideoMode();
		VideoMode(uint32_t width, uint32_t height, uint32_t bitsPerPixel = 32);
		VideoMode(const VideoMode & copy);
		~VideoMode();

		static VideoMode getDesktopMode();
		static std::vector<VideoMode> getFullscreenModes();

		bool isValid() const;

		uint32_t width;
		uint32_t height;
		uint32_t bitsPerPixel;

		inline vec2u size() const { return vec2u(width, height); }

		bool equals(const VideoMode & value) const override;		
		bool lessThan(const VideoMode & value) const override;

	};
}

#endif // !_VIDEO_MODE_H_
