#include <MemeWindow/VideoMode.h>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif // ML_SYSTEM_WINDOWS

namespace ml
{
	VideoMode::VideoMode()
		: VideoMode(vec2u::Zero, 0)
	{
	}

	VideoMode::VideoMode(uint32_t width, uint32_t height, uint32_t bitsPerPixel)
		: VideoMode(vec2u(width, height), bitsPerPixel)
	{
	}

	VideoMode::VideoMode(const vec2u & size, uint32_t bitsPerPixel)
		: size(size)
		, bitsPerPixel(bitsPerPixel)
	{
	}

	VideoMode::VideoMode(const VideoMode & copy)
		: VideoMode(copy.size, copy.bitsPerPixel)
	{
	}

	VideoMode::~VideoMode()
	{
	}


	const VideoMode & VideoMode::desktop()
	{
		static VideoMode temp;
		static bool checked = false;
		if(!checked)
		{	checked = true;

#ifdef ML_SYSTEM_WINDOWS
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &win32Mode);
			temp = VideoMode(
				win32Mode.dmPelsWidth,
				win32Mode.dmPelsHeight,
				win32Mode.dmBitsPerPel);
#else
			temp = VideoMode();
#endif
		}
		return temp;
	}

	const List<VideoMode> & VideoMode::resolutions()
	{
		static List<VideoMode> temp;
		static bool checked = false;
		if(!checked)
		{	checked = true;

#ifdef ML_SYSTEM_WINDOWS
			DEVMODE win32Mode;
			win32Mode.dmSize = sizeof(win32Mode);
			for (int32_t count = 0; EnumDisplaySettings(NULL, count, &win32Mode); ++count)
			{
				VideoMode mode(
					win32Mode.dmPelsWidth,
					win32Mode.dmPelsHeight,
					win32Mode.dmBitsPerPel);

				if (std::find(temp.begin(), temp.end(), mode) == temp.end())
				{
					temp.push_back(mode);
				}
			}
#else
			temp = List<VideoMode>();
#endif
		}
		return temp;
	}

	bool VideoMode::isValidFullscreenMode() const
	{
		static const List<VideoMode> & modes = resolutions();

		return std::find(modes.begin(), modes.end(), *this) != modes.end();
	}
}