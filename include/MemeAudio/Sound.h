#ifndef _SOUND_H_
#define _SOUND_H_

#include <MemeAudio/Export.h>

namespace ml
{
	class ML_AUDIO_API Sound
	{
	public:
		Sound();
		~Sound();

	private:
		void * m_device;
	};
}

#endif // !_SOUND_H_
