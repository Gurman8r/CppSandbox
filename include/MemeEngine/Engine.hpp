#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/Timer.hpp>

#define ML_Engine ml::Engine::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Application;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public ITrackable
		, public ISingleton<Engine>
	{
		friend class ISingleton<Engine>;

	private:
		Engine();
		~Engine();

	public:
		Application * launchApp(Application * app);
		
		int32_t freeApp(Application * value);
		
		bool isRunning() const;

		void beginFrame();

		void endFrame();

	public:
		template <
			class Fun,
			class ... Args
		> inline void loop(Fun && fun, Args && ... args)
		{
			while (this->isRunning())
			{
				this->beginFrame();
				fun((args)...);
				this->endFrame();
			}
		}

	public:
		inline const Application *	app()		const	{ return m_app;			}
		inline		 Application *	app()				{ return m_app;			}
		inline const Timer &		timer()		const	{ return m_timer;		}
		inline const Duration &		frameTime()	const	{ return m_frameTime;	}
		inline const uint32_t		frameRate()	const	{ return m_frameRate;	}

	private:
		Application * m_app;
		
		Timer		m_timer;
		Duration	m_frameTime;
		uint32_t	m_frameCount	= 0;
		uint32_t	m_frameRate		= 0;
		float		m_nextSecond	= 0;
		float		m_prevSecond	= 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_