#ifndef _SOUND_H_
#define _SOUND_H_

#include <MemeAudio/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>

namespace ml
{
	class ML_AUDIO_API Sound
		: public ITrackable
		, public IResource
	{
	public:
		Sound();
		~Sound();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

	private:
		void * m_device;
	};
}

#endif // !_SOUND_H_
