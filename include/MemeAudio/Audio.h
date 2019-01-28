#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <MemeCore/ISingleton.h>
#include <MemeAudio/Sound.h>
#include <MemeCore/IHandle.h>

/* * * * * * * * * * * * * * * * * * * * */

#ifdef ML_DEBUG
	#define alCheck(expr) do { expr; ml::Audio::checkError(__FILE__, __LINE__, #expr); } while (false)
#else
	#define alCheck(expr) (expr)
#endif // ML_DEBUG

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct ML_AUDIO_API AudioBuffer
		: public ITrackable
		, public IHandle<uint32_t>
	{
		size_t count;
		size_t size;

		AudioBuffer()
			: IHandle(NULL)
			, count(0)
			, size(0)
		{
		}
		AudioBuffer(size_t count, size_t size)
			: IHandle(NULL)
			, count(count)
			, size(size)
		{
		}
		AudioBuffer(const AudioBuffer & copy)
			: IHandle(copy)
			, count(copy.count)
			, size(copy.size)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_AUDIO_API AudioSource
		: public ITrackable
		, public IHandle<uint32_t>
	{
		size_t count;
		size_t size;

		AudioSource()
			: IHandle(NULL)
			, count(0)
			, size(0)
		{
		}
		AudioSource(size_t count, size_t size)
			: IHandle(NULL)
			, count(count)
			, size(size)
		{
		}
		AudioSource(const AudioSource & copy)
			: IHandle(copy)
			, count(copy.count)
			, size(copy.size)
		{
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Audio ml::Audio::getInstance()

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

		bool genBuffer(AudioBuffer & buf, size_t count, size_t size);
		bool genSource(AudioSource & src, size_t count, size_t size);

		void serialize(std::ostream & out) const override;

	public:
		inline bool	good() const { return (m_device && m_context); }

	private:
		Audio();
		~Audio();

		void * m_device;
		void * m_context;
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_AUDIO_MANAGER_H_
