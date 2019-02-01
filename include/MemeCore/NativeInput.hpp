#ifndef _NATIVE_INPUT_HPP_
#define _NATIVE_INPUT_HPP_

#include <MemeCore/NativeKey.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/InputState.hpp>

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

#endif // !_NATIVE_INPUT_HPP_
