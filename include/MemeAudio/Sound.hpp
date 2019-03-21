#ifndef _ML_SOUND_HPP_
#define _ML_SOUND_HPP_

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
		bool loadFromFile(const String & filename) override;

	private:
		void * m_device;
	};
}

#endif // !_ML_SOUND_HPP_