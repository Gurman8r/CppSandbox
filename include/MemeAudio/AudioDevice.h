#ifndef _AUDIO_DEVICE_H_
#define _AUDIO_DEVICE_H_

#include <MemeAudio/Export.h>
#include <MemeCore/ITrackable.h>

namespace ml
{
	class ML_AUDIO_API AudioDevice
		: public ITrackable
	{
	public:
		AudioDevice();
		~AudioDevice();

	private:

	};
}

#endif // !_AUDIO_DEVICE_H_
