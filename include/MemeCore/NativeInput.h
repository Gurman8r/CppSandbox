#ifndef _NATIVE_INPUT_H_
#define _NATIVE_INPUT_H_

#include <MemeCore/NativeKey.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/InputState.h>

namespace ml
{
	class ML_CORE_API NativeInput
		: public ITrackable
		, public InputState<NativeKey::MAX_KEYCODE>
	{
	public:
		NativeInput() {}

		~NativeInput() {}

		bool checkKey(int32_t value) const override;

		bool getAnyKey() const;
	};
}

#endif // !_NATIVE_INPUT_H_