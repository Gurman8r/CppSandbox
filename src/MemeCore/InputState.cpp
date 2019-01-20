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
		auto checkKey = [](uint32_t key)
		{
#ifdef ML_SYSTEM_WINDOWS
			return (bool)GetAsyncKeyState((int32_t)key);
#else
			return false;
#endif
		};

		for (uint32_t i = 0; i < KeyCode::MAX_KEYCODE; i++)
		{
			m_new[i] = checkKey(i);
		}
		return (*this);
	}

	InputState & InputState::endStep()
	{
		memcpy(m_old, m_new, KeyCode::MAX_KEYCODE);
		//for (uint32_t i = 0; i < KeyCode::MAX_KEYCODE; i++)
		//{
		//	m_old[i] = m_new[i];
		//}
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
