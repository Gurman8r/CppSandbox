#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <MemeCore/ISingleton.h>
#include <MemeAudio/Sound.h>

#define ML_Audio ml::Audio::getInstance()

#ifdef ML_DEBUG
	#define alCheck(expr) do { expr; ml::Audio::checkError(__FILE__, __LINE__, #expr); } while (false)
#else
	#define alCheck(expr) (expr)
#endif // ML_DEBUG


namespace ml
{
	class ML_AUDIO_API Audio
		: public ITrackable
		, public ISingleton<Audio>
	{
		friend class ISingleton<Audio>;

	public:
		enum Error
		{
			NoError,
			InvalidName,
			InvalidEnum,
			InvalidValue,
			InvalidOperation,
			OutOfMemory,
			
			MAX_ERROR
		};

	public:
		static Error	getError();
		static void		checkError(const char * file, uint32_t line, const char * expr);

	public:
		bool openDevice();
		bool closeDevice();
		bool createContext();
		bool makeContextCurrent();
		bool init();

	private:
		Audio();
		~Audio();

		void * m_device;
		void * m_context;
	};
}

#endif // !_AUDIO_MANAGER_H_
