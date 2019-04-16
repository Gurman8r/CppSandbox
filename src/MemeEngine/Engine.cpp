#include <MemeEngine/Engine.hpp>
#include <MemeEngine/Application.hpp>
#include <MemeCore/Time.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Engine::Engine()
		: m_app(NULL)
	{
	}

	Engine::~Engine()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Application * Engine::launchApp(Application * value)
	{
		return ((!m_app) ? (m_app = value) : (NULL));
	}

	int32_t Engine::freeApp(Application * value)
	{
		if (m_app && (m_app == value))
		{
			delete m_app;
			m_app = NULL;
		}
		return ((m_app) ? (EXIT_FAILURE) : (EXIT_SUCCESS));
	}

	bool Engine::isRunning() const
	{
		return (m_app && m_app->isOpen());
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Engine::beginFrame()
	{
		m_timer.start();
		if (m_app)
		{
			m_app->pollEvents();
		}
	}

	void Engine::endFrame()
	{
		if (m_app)
		{
			m_app->swapBuffers();
		}
		m_elapsed = m_timer.stop().elapsed();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}