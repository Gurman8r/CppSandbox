#include <MemeWindow/WindowInput.hpp>
#include <MemeWindow/Window.hpp>

namespace ml
{
	bool WindowInput::checkKey(int32_t value) const
	{
		if (m_window)
		{
			return m_window->getKey(value);
		}
		return false;
	}

	bool WindowInput::getAnyKey() const
	{
		for (int32_t i = WindowKey::MIN_KEYCODE; i < WindowKey::MAX_KEYCODE; i++)
		{
			if (getKey(i))
			{
				return true;
			}
		}
		return false;
	}
}