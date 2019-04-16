#ifndef _ML_SOUND_HPP_
#define _ML_SOUND_HPP_

#include <MemeAudio/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IReadable.hpp>
#include <MemeCore/IDisposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API Sound
		: public ITrackable
		, public IDisposable
		, public IReadable
	{
	public:
		Sound();
		~Sound();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	private:
		void * m_device;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SOUND_HPP_