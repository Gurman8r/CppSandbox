#include <MemeLib/InputState.h>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

namespace PRIV
{
	inline bool _getKeyState(int32_t value)
	{
#ifdef _WIN32
		return (bool)GetKeyState(value);
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


	void InputState::beginStep()
	{
		for (uint32_t i = 0; i < KeyCode::MAX_KEYCODE; i++)
		{
			m_new[i] = PRIV::_getKeyState((int32_t)i);
		}
	}

	void InputState::endStep()
	{
		for (uint32_t i = 0; i < KeyCode::MAX_KEYCODE; i++)
		{
			m_old[i] = m_new[i];
		}
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
}
