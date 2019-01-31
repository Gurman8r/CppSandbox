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
		/* Initialization */
		static bool init();
		static bool good();

	public:
		/* Errors */
		static AL::Err	getError();
		static void		checkError(const char * file, uint32_t line, const char * expr);

	public:
		/* Devices */
		static bool		openDevice();
		static bool		closeDevice();

	public:
		/* Context */
		static bool		createContext();
		static bool		makeContextCurrent();

	public:
		/* Buffers */
		static uint32_t	genBuffers(int32_t count);
		static void		deleteBuffers(size_t count, const uint32_t * buffers);
		static bool		isBuffer(uint32_t id);
		static void		bufferData(uint32_t id, AL::Enum format, const void * data, int32_t size, int32_t freq);
		
		/* Set Buffer parameters */
		static void		bufferf(uint32_t id, AL::Enum param, float value);
		static void		buffer3f(uint32_t id, AL::Enum param, float x, float y, float z);
		static void		bufferfv(uint32_t id, AL::Enum param, const float * values);
		static void		bufferi(uint32_t id, AL::Enum param, int32_t value);
		static void		buffer3i(uint32_t id, AL::Enum param, int32_t x, int32_t y, int32_t z);
		static void		bufferiv(uint32_t id, AL::Enum param, const int32_t * values);

		/* Get Buffer parameters */
		static float	getBufferf(uint32_t id, AL::Enum param);
		static float *	getBuffer3f(uint32_t id, AL::Enum param);
		static float *	getBufferfv(uint32_t id, AL::Enum param);
		static int32_t	getBufferi(uint32_t id, AL::Enum param);
		static int32_t *getBuffer3i(uint32_t id, AL::Enum param);
		static int32_t *getBufferiv(uint32_t id, AL::Enum param);

		/* Global Parameters */
		static void		dopplerFactor(float value);
		static void		dopplerVelocity(float value);
		static void		speedOfSound(float value);
		static void		distanceModel(AL::Enum value);

	public:
		/* Sources */
		static uint32_t	genSources(int32_t count);
		static void		deleteSources(int32_t count, const uint32_t * sources);
		static bool		isSource(uint32_t id);

		/* Set Source parameters */
		static void		sourcef(uint32_t id, AL::Enum param, float value);
		static void		source3f(uint32_t id, AL::Enum param, float x, float y, float z);
		static void		sourcefv(uint32_t id, AL::Enum param, const float * values);
		static void		sourcei(uint32_t id, AL::Enum param, int32_t value);
		static void		source3i(uint32_t id, AL::Enum param, int32_t x, int32_t y, int32_t z);
		static void		sourceiv(uint32_t id, AL::Enum param, const int32_t * values);

		/* Get Source parameters */
		static float	getSourcef(uint32_t id, AL::Enum param);
		static float *	getSource3f(uint32_t id, AL::Enum param);
		static float *	getSourcefv(uint32_t id, AL::Enum param);
		static int32_t	getSourcei(uint32_t id, AL::Enum param);
		static int32_t *getSource3i(uint32_t id, AL::Enum param);
		static int32_t *getSourceiv(uint32_t id, AL::Enum param);

		/* Source based playback calls */
		static void		sourcePlay(uint32_t id);
		static void		sourceStop(uint32_t id);
		static void		sourceRewind(uint32_t id);
		static void		sourcePause(uint32_t id);

		/* Source vector based playback calls */
		static void		sourcePlay(int32_t count, const uint32_t * ids);
		static void		sourceStop(int32_t count, const uint32_t * ids);
		static void		sourceRewind(int32_t count, const uint32_t * ids);
		static void		sourcePause(int32_t count, const uint32_t * ids);

	public:
		/* Set Listener parameters */
		static void		listenerf(AL::Enum param, float value);
		static void		listener3f(AL::Enum param, float x, float y, float z);
		static void		listenerfv(AL::Enum param, const float * values);
		static void		listeneri(AL::Enum param, int32_t value);
		static void		listener3i(AL::Enum param, int32_t x, int32_t y, int32_t z);
		static void		listeneriv(AL::Enum param, const int32_t * values);

		/* Get Source parameters */
		static float	getListenerf(AL::Enum param);
		static float *	getListener3f(AL::Enum param);
		static float *	getListenerfv(AL::Enum param);
		static int32_t	getListeneri(AL::Enum param);
		static int32_t *getListener3i(AL::Enum param);
		static int32_t *getListeneriv(AL::Enum param);
		
	private:
		static void * m_device;
		static void * m_context;
	};
}

#endif // !_OPEN_AL_H_
