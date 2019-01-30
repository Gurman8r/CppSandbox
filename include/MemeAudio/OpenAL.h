#ifndef _OPEN_AL_H_
#define _OPEN_AL_H_

#include <MemeAudio/AL.h>

#ifdef ML_DEBUG
	#define alCheck(expr) do { expr; ml::OpenAL::checkError(__FILE__, __LINE__, #expr); } while (false)
#else
	#define alCheck(expr) (expr)
#endif // ML_DEBUG

namespace ml
{
	class ML_AUDIO_API OpenAL final
	{
	public:
		static bool init();
		static bool good();

	public:
		static AL::Err	getError();
		static void		checkError(const char * file, uint32_t line, const char * expr);

	public:
		static bool		openDevice();
		static bool		closeDevice();
		static bool		createContext();
		static bool		makeContextCurrent();

	public:
		static void		genBuffers(size_t count, size_t size, uint32_t * handle);
		static void		genSources(size_t count, size_t size, uint32_t * handle);
		
	private:
		static void * m_device;
		static void * m_context;
	};
}

#endif // !_OPEN_AL_H_
