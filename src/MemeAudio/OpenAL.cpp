#include <MemeAudio/OpenAL.hpp>
#include <MemeCore/Debug.hpp>
#include <AL/al.h>
#include <AL/alc.h>

#define DEVICE(ptr) static_cast<ALCdevice *>(ptr)
#define CONTEXT(ptr) static_cast<ALCcontext *>(ptr)

namespace ml
{
	/* Members */

	void * OpenAL::m_device = NULL;
	void * OpenAL::m_context = NULL;


	/* Initialization */

	bool OpenAL::init()
	{
		static bool check = false;
		if (!check && !good())
		{
			check =
				openDevice() &&
				createContext() &&
				makeContextCurrent();
		}
		return check;
	}
	
	bool OpenAL::good()
	{
		return (m_device && m_context);
	}
	

	/* Errors */
	
	AL::Err OpenAL::getError()
	{
		return static_cast<AL::Err>(alGetError());
	}

	void OpenAL::checkError(CString file, uint32_t line, CString expr)
	{
		if (AL::Err errorCode = getError())
		{
			String fileName(file);
			fileName = fileName.substr(fileName.find_last_of("\\/") + 1);

			// Decode the error code
			String errorName;
			String errorDesc;
			switch (errorCode)
			{
			case AL::InvalidName:
				errorName = "Invalid Name";
				errorDesc = "An unacceptable value has been specified for a name argument.";
				break;
			case AL::InvalidEnum:
				errorName = "Invalid Enum";
				errorDesc = "An unacceptable value has been specified for an enumerated argument.";
				break;
			case AL::InvalidValue:
				errorName = "Invalid value_type";
				errorDesc = "A numeric argument is out of range.";
				break;
			case AL::InvalidOperation:
				errorName = "Invalid Operation";
				errorDesc = "The specified operation is not allowed in the current state.";
				break;
			case AL::OutOfMemory:
				errorName = "Out Of Memory";
				errorDesc = "There is not enough memory left to execute the command.";
				break;
			default:
				errorName = "Unknown error";
				errorDesc = "No description";
				break;
			}

			cerr
				<< FMT()
				<< ml::endl
				<< FG::Red
				<< "An internal OpenAL call failed in " << fileName << "(" << line << ")"
				<< FG::Yellow << ml::endl << "Code: "
				<< FG::White << ml::endl << "\t" << errorCode
				<< FG::Yellow << ml::endl << "Expression: "
				<< FG::White << ml::endl << "\t" << expr
				<< FG::Yellow << ml::endl << "Description:"
				<< FG::White << ml::endl << "\t" << errorName
				<< FG::White << ml::endl << "\t" << errorDesc
				<< FMT()
				<< ml::endl
				<< ml::endl;
		}
	}
	

	/* Devices */
	
	bool OpenAL::openDevice()
	{
		static bool check = false;
		if (!check && !m_device)
		{
			alCheck(check = (m_device = DEVICE(alcOpenDevice(NULL))));
		}
		return check;
	}
	
	bool OpenAL::closeDevice()
	{
		static bool check = false;
		if (!check && m_device)
		{
			alCheck(check = alcCloseDevice(DEVICE(m_device)));
		}
		return check;
	}


	/* Context */
	
	bool OpenAL::createContext()
	{
		static bool check = false;
		if (!check && m_device && !m_context)
		{
			alCheck(check = (m_context = CONTEXT(alcCreateContext(DEVICE(m_device), NULL))));
		}
		return check;
	}
	
	bool OpenAL::makeContextCurrent()
	{
		static bool check = false;
		if (!check && m_context)
		{
			alCheck(check = alcMakeContextCurrent(CONTEXT(m_context)));
		}
		return check;
	}


	/* Buffers */

	uint32_t OpenAL::genBuffers(AL::Size count)
	{
		static uint32_t temp;
		alCheck(alGenBuffers(count, &temp));
		return temp;
	}

	void OpenAL::deleteBuffers(AL::Size count, const uint32_t * buffers)
	{
		alCheck(alDeleteBuffers(count, buffers));
	}

	bool OpenAL::isBuffer(uint32_t id)
	{
		static bool temp;
		alCheck(temp = alIsBuffer(id));
		return temp;
	}

	void OpenAL::bufferData(uint32_t id,AL::Enum format, const void * data, AL::Size size, int32_t freq)
	{
		alCheck(alBufferData(id, format, data, size, freq));
	}


	/* Set Buffer Parameters */

	void OpenAL::bufferf(uint32_t id,AL::Enum param, float value)
	{
		alCheck(alBufferf(id, param, value));
	}

	void OpenAL::buffer3f(uint32_t id,AL::Enum param, float x, float y, float z)
	{
		alCheck(alBuffer3f(id, param, x, y, z));
	}

	void OpenAL::bufferfv(uint32_t id,AL::Enum param, const float * values)
	{
		alCheck(alBufferfv(id, param, values));
	}

	void OpenAL::bufferi(uint32_t id,AL::Enum param, int32_t value)
	{
		alCheck(alBufferi(id, param, value));
	}

	void OpenAL::buffer3i(uint32_t id,AL::Enum param, int32_t x, int32_t y, int32_t z)
	{
		alCheck(alBuffer3i(id, param, x, y, z));
	}

	void OpenAL::bufferiv(uint32_t id,AL::Enum param, const int32_t * values)
	{
		alCheck(alBufferiv(id, param, values));
	}


	/* Get Buffer Parameters */

	float		OpenAL::getBufferf(uint32_t id,AL::Enum param)
	{
		static float temp;
		alCheck(alGetBufferf(id, param, &temp));
		return temp;
	}

	float *		OpenAL::getBuffer3f(uint32_t id,AL::Enum param)
	{
		static float temp[3];
		alCheck(alGetBuffer3f(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float *		OpenAL::getBufferfv(uint32_t id,AL::Enum param)
	{
		static float * temp;
		alCheck(alGetBufferfv(id, param, temp));
		return temp;
	}

	int32_t		OpenAL::getBufferi(uint32_t id,AL::Enum param)
	{
		static int32_t temp;
		alCheck(alGetBufferi(id, param, &temp));
		return temp;
	}

	int32_t *	OpenAL::getBuffer3i(uint32_t id,AL::Enum param)
	{
		static int32_t temp[3];
		alCheck(alGetBuffer3i(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	int32_t *	OpenAL::getBufferiv(uint32_t id,AL::Enum param)
	{
		static int32_t * temp;
		alCheck(alGetBufferiv(id, param, temp));
		return temp;
	}


	/* Global Parameters */

	void OpenAL::dopplerFactor(float value)
	{
		alCheck(alDopplerFactor(value));
	}

	void OpenAL::dopplerVelocity(float value)
	{
		alCheck(alDopplerVelocity(value));
	}

	void OpenAL::speedOfSound(float value)
	{
		alCheck(alSpeedOfSound(value));
	}

	void OpenAL::distanceModel(AL::Enum value)
	{
		alCheck(alDistanceModel(value));
	}


	/* Sources */

	uint32_t OpenAL::genSources(AL::Size count)
	{
		static uint32_t temp;
		alCheck(alGenSources(count, &temp));
		return temp;
	}

	void OpenAL::deleteSources(AL::Size count, const uint32_t * sources)
	{
		alCheck(alDeleteSources(count, sources));
	}

	bool OpenAL::isSource(uint32_t id)
	{
		static bool temp;
		alCheck(temp = alIsSource(id));
		return temp;
	}


	/* Set Source Parameters */

	void OpenAL::sourcef(uint32_t id,AL::Enum param, float value)
	{
		alCheck(alSourcef(id, param, value));
	}

	void OpenAL::source3f(uint32_t id,AL::Enum param, float x, float y, float z)
	{
		alCheck(alSource3f(id, param, x, y, z));
	}

	void OpenAL::sourcefv(uint32_t id,AL::Enum param, const float * values)
	{
		alCheck(alSourcefv(id, param, values));
	}

	void OpenAL::sourcei(uint32_t id,AL::Enum param, int32_t value)
	{
		alCheck(alSourcei(id, param, value));
	}

	void OpenAL::source3i(uint32_t id,AL::Enum param, int32_t x, int32_t y, int32_t z)
	{
		alCheck(alSource3i(id, param, x, y, z));
	}

	void OpenAL::sourceiv(uint32_t id,AL::Enum param, const int32_t * values)
	{
		alCheck(alSourceiv(id, param, values));
	}


	/* Get Source Parameters */

	float		OpenAL::getSourcef(uint32_t id,AL::Enum param)
	{
		static float temp;
		alCheck(alGetSourcef(id, param, &temp));
		return temp;
	}

	float *		OpenAL::getSource3f(uint32_t id,AL::Enum param)
	{
		static float temp[3];
		alCheck(alGetSource3f(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float *		OpenAL::getSourcefv(uint32_t id,AL::Enum param)
	{
		static float * temp;
		alCheck(alGetSourcefv(id, param, temp));
		return temp;
	}

	int32_t		OpenAL::getSourcei(uint32_t id,AL::Enum param)
	{
		static int32_t temp;
		alCheck(alGetSourcei(id, param, &temp));
		return temp;
	}

	int32_t *	OpenAL::getSource3i(uint32_t id,AL::Enum param)
	{
		static int32_t temp[3];
		alCheck(alGetSource3i(id, param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	int32_t *	OpenAL::getSourceiv(uint32_t id,AL::Enum param)
	{
		static int32_t * temp;
		alCheck(alGetSourceiv(id, param, temp));
		return temp;
	}


	/* Source based playback calls */

	void OpenAL::sourcePlay(uint32_t id)
	{
		alCheck(alSourcePlay(id));
	}

	void OpenAL::sourceStop(uint32_t id)
	{
		alCheck(alSourceStop(id));
	}

	void OpenAL::sourceRewind(uint32_t id)
	{
		alCheck(alSourceRewind(id));
	}

	void OpenAL::sourcePause(uint32_t id)
	{
		alCheck(alSourcePause(id));
	}


	/* Source vector based playback calls */

	void OpenAL::sourcePlay(AL::Size count, const uint32_t * ids)
	{
		alCheck(alSourcePlayv(count, ids));
	}

	void OpenAL::sourceStop(AL::Size count, const uint32_t * ids)
	{
		alCheck(alSourceStopv(count, ids));
	}

	void OpenAL::sourceRewind(AL::Size count, const uint32_t * ids)
	{
		alCheck(alSourceRewindv(count, ids));
	}

	void OpenAL::sourcePause(AL::Size count, const uint32_t * ids)
	{
		alCheck(alSourcePausev(count, ids));
	}


	/* Set Listener Parameters */

	void OpenAL::listenerf(AL::Enum param, float value)
	{
		alCheck(alListenerf(param, value));
	}

	void OpenAL::listener3f(AL::Enum param, float x, float y, float z)
	{
		alCheck(alListener3f(param, x, y, z));
	}

	void OpenAL::listenerfv(AL::Enum param, const float * values)
	{
		alCheck(alListenerfv(param, values));
	}

	void OpenAL::listeneri(AL::Enum param, int32_t value)
	{
		alCheck(alListeneri(param, value));
	}

	void OpenAL::listener3i(AL::Enum param, int32_t x, int32_t y, int32_t z)
	{
		alCheck(alListener3i(param, x, y, z));
	}

	void OpenAL::listeneriv(AL::Enum param, const int32_t * values)
	{
		alCheck(alListeneriv(param, values));
	}


	/* Get Listener Parameters */

	float		OpenAL::getListenerf(AL::Enum param)
	{
		static float temp;
		alCheck(alGetListenerf(param, &temp));
		return temp;
	}

	float *		OpenAL::getListener3f(AL::Enum param)
	{
		static float temp[3];
		alCheck(alGetListener3f(param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	float *		OpenAL::getListenerfv(AL::Enum param)
	{
		static float * temp;
		alCheck(alGetListenerfv(param, temp));
		return temp;
	}

	int32_t		OpenAL::getListeneri(AL::Enum param)
	{
		static int32_t temp;
		alCheck(alGetListeneri(param, &temp));
		return temp;
	}

	int32_t *	OpenAL::getListener3i(AL::Enum param)
	{
		static int32_t temp[3];
		alCheck(alGetListener3i(param, &temp[0], &temp[1], &temp[2]));
		return temp;
	}

	int32_t *	OpenAL::getListeneriv(AL::Enum param)
	{
		static int32_t * temp;
		alCheck(alGetListeneriv(param, temp));
		return temp;
	}
}