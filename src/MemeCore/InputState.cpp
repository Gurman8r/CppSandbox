#include <MemeCore/InputState.h>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

namespace PRIV
{
	inline static bool _getKeyState(int32_t value)
	{
#ifdef _WIN32
		return (bool)GetAsyncKeyState(value);
#else
		return false;
#endif
	}
}

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
		for (uint32_t i = 0; i < KeyCode::MAX_KEYCODE; i++)
		{
			m_new[i] = PRIV::_getKeyState((int32_t)i);
		}
		return (*this);
	}

	InputState & InputState::endStep()
	{
		for (uint32_t i = 0; i < KeyCode::MAX_KEYCODE; i++)
		{
			m_old[i] = m_new[i];
		}
		return (*this);
	}


	bool InputState::getKey(KeyCode value) const
	{
		return (m_new[value]);
	}

	bool InputState::getKeyDown(KeyCode value) const
	{
		return (m_new[value]) && (!m_old[value]);
	}

	bool InputState::getKeyUp(KeyCode value) const
	{
		return (!m_new[value]) && (m_old[value]);
	}
	
	
	bool InputState::getAnyKey(bool allowMouse) const
	{
		uint32_t imin = (allowMouse ?  KeyCode::LeftMouse : KeyCode::Backspace);
		for (uint32_t i = imin; i < KeyCode::MAX_KEYCODE; i++)
		{
			if (getKey((KeyCode)i)) return true;
		}
		return false;
	}
}
