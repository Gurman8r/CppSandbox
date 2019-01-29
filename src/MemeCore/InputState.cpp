#include <MemeCore/InputState.h>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif // ML_SYSTEM_WINDOWS

namespace ml
{
	InputState::InputState()
	{
	}

	InputState::~InputState()
	{
	}


	InputState & InputState::beginStep()
	{
		for (uint32_t i = 0; i < Key::Code::MAX_KEYCODE; i++)
		{
#ifdef ML_SYSTEM_WINDOWS
			m_new[i] = (bool)GetAsyncKeyState((int32_t)i);
#else
			m_new[i] = false;
#endif
		}
		return (*this);
	}

	InputState & InputState::endStep()
	{
		memcpy(m_old, m_new, Key::Code::MAX_KEYCODE);
		return (*this);
	}


	bool InputState::getKey(Key::Code value) const
	{
		return (m_new[value]);
	}

	bool InputState::getKeyDown(Key::Code value) const
	{
		return (m_new[value]) && (!m_old[value]);
	}

	bool InputState::getKeyUp(Key::Code value) const
	{
		return (!m_new[value]) && (m_old[value]);
	}
	
	
	bool InputState::getAnyKey(bool allowMouse) const
	{
		uint32_t imin = (allowMouse ?  Key::Code::LeftMouse : Key::Code::Backspace);
		for (uint32_t i = imin; i < Key::Code::MAX_KEYCODE; i++)
		{
			if (getKey((Key::Code)i)) return true;
		}
		return false;
	}
}
