#ifndef _INPUT_H_
#define _INPUT_H_

#include <MemeCore/KeyCode.h>
#include <MemeCore/ITrackable.h>

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

		bool getKey(Key::Code value) const;
		bool getKeyDown(Key::Code value) const;
		bool getKeyUp(Key::Code value) const;

		bool getAnyKey(bool allowMouse = false) const;

	private:
		bool m_new[Key::Code::MAX_KEYCODE];
		bool m_old[Key::Code::MAX_KEYCODE];
	};
}

#endif // !_INPUT_H_