#ifndef _ML_OPEN_AL_HPP_
#define _ML_OPEN_AL_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <MemeAudio/AL.hpp>
#include <MemeCore/String.hpp>
#include <MemeCore/ISingleton.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_AL ml::OpenAL::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

# if ML_DEBUG
#	define alCheck(expr) do { expr; ML_AL.checkError(__FILE__, __LINE__, #expr); } while (false)
# else
#	define alCheck(expr) (expr)
# endif

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API OpenAL final
		: public ISingleton<OpenAL>
	{
		friend class ISingleton<OpenAL>;

		void * m_device;
		void * m_context;

	public:
		/* Initialization */
		static bool init();
		static bool good();

	public:
		/* Errors */
		static AL::Err	getError();
		static void		checkError(CString file, uint32_t line, CString expr);

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
		static void		deleteBuffers(int32_t count, const uint32_t * buffers);
		static bool		isBuffer(uint32_t id);
		static void		bufferData(uint32_t id, int32_t format, const void * data, int32_t size, int32_t freq);
		
		/* Set Buffer parameters */
		static void		bufferf(uint32_t id, int32_t param, float value);
		static void		buffer3f(uint32_t id, int32_t param, float x, float y, float z);
		static void		bufferfv(uint32_t id, int32_t param, const float * values);
		static void		bufferi(uint32_t id, int32_t param, int32_t value);
		static void		buffer3i(uint32_t id, int32_t param, int32_t x, int32_t y, int32_t z);
		static void		bufferiv(uint32_t id, int32_t param, const int32_t * values);

		/* Get Buffer parameters */
		static float	getBufferf(uint32_t id, int32_t param);
		static float *	getBuffer3f(uint32_t id, int32_t param);
		static float *	getBufferfv(uint32_t id, int32_t param);
		static int32_t	getBufferi(uint32_t id, int32_t param);
		static int32_t *getBuffer3i(uint32_t id, int32_t param);
		static int32_t *getBufferiv(uint32_t id, int32_t param);

		/* Global Parameters */
		static void		dopplerFactor(float value);
		static void		dopplerVelocity(float value);
		static void		speedOfSound(float value);
		static void		distanceModel(int32_t value);

	public:
		/* Sources */
		static uint32_t	genSources(int32_t count);
		static void		deleteSources(int32_t count, const uint32_t * sources);
		static bool		isSource(uint32_t id);

		/* Set Source parameters */
		static void		sourcef(uint32_t id, int32_t param, float value);
		static void		source3f(uint32_t id, int32_t param, float x, float y, float z);
		static void		sourcefv(uint32_t id, int32_t param, const float * values);
		static void		sourcei(uint32_t id, int32_t param, int32_t value);
		static void		source3i(uint32_t id, int32_t param, int32_t x, int32_t y, int32_t z);
		static void		sourceiv(uint32_t id, int32_t param, const int32_t * values);

		/* Get Source parameters */
		static float	getSourcef(uint32_t id, int32_t param);
		static float *	getSource3f(uint32_t id, int32_t param);
		static float *	getSourcefv(uint32_t id, int32_t param);
		static int32_t	getSourcei(uint32_t id, int32_t param);
		static int32_t *getSource3i(uint32_t id, int32_t param);
		static int32_t *getSourceiv(uint32_t id, int32_t param);

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
		static void		listenerf(int32_t param, float value);
		static void		listener3f(int32_t param, float x, float y, float z);
		static void		listenerfv(int32_t param, const float * values);
		static void		listeneri(int32_t param, int32_t value);
		static void		listener3i(int32_t param, int32_t x, int32_t y, int32_t z);
		static void		listeneriv(int32_t param, const int32_t * values);

		/* Get Source parameters */
		static float	getListenerf(int32_t param);
		static float *	getListener3f(int32_t param);
		static float *	getListenerfv(int32_t param);
		static int32_t	getListeneri(int32_t param);
		static int32_t *getListener3i(int32_t param);
		static int32_t *getListeneriv(int32_t param);
		
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_OPEN_AL_HPP_