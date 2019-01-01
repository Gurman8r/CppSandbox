#include <MemeWindow/Screen.h>	
#include <MemeWindow/VideoMode.h>

namespace ml
{
	vec2u Screen::size()
	{
		return VideoMode::getDesktopMode().size();
	}
}