#ifndef _SOUND_HPP_
#define _SOUND_HPP_

#include <MemeAudio/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IReadable.hpp>

namespace ml
{
	class ML_AUDIO_API Sound
		: public ITrackable
		, public IReadable
	{
	public:
		Sound();
		~Sound();

		bool cleanup() override;
		bool readFile(const String & filename) override;

	private:
		void * m_device;
	};
}

#endif // !_SOUND_HPP_
