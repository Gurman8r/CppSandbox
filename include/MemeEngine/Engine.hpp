#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>
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
		int32_t freeApp();

		void beginFrame();
		void endFrame();

	public:
		inline const Application *	app()		{ return m_app; }
		inline const Timer &		timer()		{ return m_timer; }
		inline const Duration &		elapsed()	{ return m_elapsed; }

	private:
		Application *	m_app;
		Timer			m_timer;
		Duration		m_elapsed;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_