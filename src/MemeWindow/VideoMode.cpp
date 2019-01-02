#include <MemeWindow/VideoMode.h>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif // ML_SYSTEM_WINDOWS

namespace ml
{
	VideoMode::VideoMode()
		: width(0)
		, height(0)
		, bitsPerPixel(0)
	{
	}

	VideoMode::VideoMode(uint32_t width, uint32_t height, uint32_t bitsPerPixel)
		: width(width)
		, height(height)
		, bitsPerPixel(bitsPerPixel)
	{
	}

	VideoMode::VideoMode(const VideoMode & copy)
		: width(copy.width)
		, height(copy.height)
		, bitsPerPixel(copy.bitsPerPixel)
	{
	}

	VideoMode::~VideoMode()
	{
	}


	VideoMode VideoMode::getDesktopMode()
	{
#ifdef ML_SYSTEM_WINDOWS
		DEVMODE win32Mode;
		win32Mode.dmSize = sizeof(win32Mode);
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &win32Mode);
		return VideoMode(
			win32Mode.dmPelsWidth,
			win32Mode.dmPelsHeight,
			win32Mode.dmBitsPerPel);
#else
		return VideoMode();
#endif

	}

	std::vector<VideoMode> VideoMode::getFullscreenModes()
	{
		std::vector<VideoMode> modes;

#ifdef ML_SYSTEM_WINDOWS		
		DEVMODE win32Mode;
		win32Mode.dmSize = sizeof(win32Mode);
		for (int count = 0; EnumDisplaySettings(NULL, count, &win32Mode); ++count)
		{
			VideoMode mode(
				win32Mode.dmPelsWidth,
				win32Mode.dmPelsHeight,
				win32Mode.dmBitsPerPel);
			if (std::find(modes.begin(), modes.end(), mode) == modes.end())
			{
				modes.push_back(mode);
			}
		}
#else

#endif
		return modes;
	}

	bool VideoMode::isValid() const
	{
		const std::vector<VideoMode>& modes = getFullscreenModes();

		return std::find(modes.begin(), modes.end(), *this) != modes.end();
	}
	
	
	inline bool VideoMode::equals(const VideoMode & value) const
	{
		return
			(width == value.width) &&
			(height == value.height) &&
			(bitsPerPixel == value.bitsPerPixel);
	}
	
	inline bool VideoMode::lessThan(const VideoMode & value) const
	{
		if (bitsPerPixel == value.bitsPerPixel)
		{
			if (width == value.width)
			{
				return height < value.height;
			}
			else
			{
				return width < value.width;
			}
		}
		else
		{
			return bitsPerPixel < value.bitsPerPixel;
		}
	}
}