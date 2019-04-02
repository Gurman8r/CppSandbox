#include <MemeEngine/Engine.hpp>
#include <MemeEngine/Application.hpp>

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

	Application * Engine::launchApp(Application * app)
	{
		return ((!m_app) ? (m_app = app) : (NULL));
	}

	int32_t Engine::freeApp()
	{
		if (m_app)
		{
			delete m_app;
			m_app = NULL;
			return EXIT_SUCCESS;
		}
		else
		{
			return EXIT_FAILURE;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}