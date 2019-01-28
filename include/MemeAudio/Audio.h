#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <MemeCore/ISingleton.h>
#include <MemeAudio/Sound.h>
#include <MemeCore/IHandle.h>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Audio ml::Audio::getInstance()

#ifdef ML_DEBUG
	#define alCheck(expr) do { expr; ml::Audio::checkError(__FILE__, __LINE__, #expr); } while (false)
#else
	#define alCheck(expr) (expr)
#endif // ML_DEBUG

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct ML_AUDIO_API SoundBuffer
		: public ITrackable
		, public IHandle<uint32_t>
	{
		size_t count;
		size_t size;

		SoundBuffer()
			: IHandle(NULL)
			, count(0)
			, size(0)
		{
		}
		SoundBuffer(size_t count, size_t size)
			: IHandle(NULL)
			, count(count)
			, size(size)
		{
		}
		SoundBuffer(const SoundBuffer & copy)
			: IHandle(copy)
			, count(copy.count)
			, size(copy.size)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_AUDIO_API SoundSource
		: public ITrackable
		, public IHandle<uint32_t>
	{
		size_t count;
		size_t size;

		SoundSource()
			: IHandle(NULL)
			, count(0)
			, size(0)
		{
		}
		SoundSource(size_t count, size_t size)
			: IHandle(NULL)
			, count(count)
			, size(size)
		{
		}
		SoundSource(const SoundSource & copy)
			: IHandle(copy)
			, count(copy.count)
			, size(copy.size)
		{
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

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

		bool createBuffer(SoundBuffer & buf, size_t count, size_t size);
		bool createSource(SoundSource & src, size_t count, size_t size);

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
