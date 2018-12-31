#ifndef _INPUT_H_
#define _INPUT_H_

#include <MemeLib/KeyCode.h>
#include <MemeLib/ITrackable.h>

namespace ml
{
	class ML_CORE_API InputState
		: public ITrackable
	{
	public:
		InputState();
		~InputState();

		InputState & beginStep();
		InputState & endStep();

		bool getKey(KeyCode value) const;
		bool getKeyDown(KeyCode value) const;
		bool getKeyUp(KeyCode value) const;

		bool getAnyKey(bool allowMouse = false) const;

	private:
		bool m_new[KeyCode::MAX_KEYCODE];
		bool m_old[KeyCode::MAX_KEYCODE];
	};
}

#endif // !_INPUT_H_