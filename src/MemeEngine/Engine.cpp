#include <MemeEngine/Engine.hpp>
#include <MemeEngine/Application.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/Debug.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	define SC_CLOSE 0xF060
#	define MF_GRAYED 0x00000001L
# endif // ML_SYSTEM_WINDOWS

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Engine::Engine()
		: m_app(NULL)
	{
#ifdef ML_SYSTEM_WINDOWS
		ML_Console.enableMenuItem(SC_CLOSE, MF_GRAYED);
#endif // ML_SYSTEM_WINDOWS
	}

	Engine::~Engine()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Application * Engine::launchApp(Application * value)
	{
		return ((!isRunning()) ? (m_app = value) : (NULL));
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
		return ((m_app) && (m_app->isOpen()));
	}

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