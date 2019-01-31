#include <MemeCore/NativeInput.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif // ML_SYSTEM_WINDOWS

namespace ml
{
	bool NativeInput::checkKey(int32_t value) const
	{
#ifdef ML_SYSTEM_WINDOWS
		return (bool)GetAsyncKeyState(value);
#else
		return false;
#endif
	}
	
	bool NativeInput::getAnyKey() const
	{
		for (uint32_t i = NativeKey::MIN_MOUSE; i < NativeKey::MAX_KEYCODE; i++)
		{
			if (getKey((int32_t)i))
			{
				return true;
			}
		}
		return false;
	}
}
