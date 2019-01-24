/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */

int main(int32_t argc, char ** argv)
{
	// Load Settings
	if (!SETTINGS.loadFromFile(CONFIG_INI))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Program Enter
	if (!demo::onProgramEnter({ argc, argv }))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Enable OpenGL Error Pause
	ml::OpenGL::errorPause(true);

	// Create Window
	ml::Debug::Log("Creating Window...");
	ml::RenderWindow window;
	if (!window.create(
		SETTINGS.winTitle,
		ml::VideoMode({ SETTINGS.winWidth, SETTINGS.winHeight }, 32),
		ml::Window::Style::Default,
		ml::Context(3, 3, 24, 8, ml::Context::Compat, false, false)))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Load Resources
	if (!demo::onLoadResources({ }))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Start
	demo::onStart({ window });

	// Loop
	ml::Timer		loopTimer;
	ml::Duration	elapsed;
	ml::InputState	input;
	do
	{	// Begin Step
		loopTimer.start();
		input.beginStep();
		{
			// Update
			demo::onUpdate({ window, elapsed, input });

			// Draw
			demo::onDraw({ window, elapsed });
		}
		// End Step
		input.endStep();
		elapsed = loopTimer.stop().elapsed();
	}
	while (window.isOpen());

	// Program Exit
	return demo::onProgramExit({ EXIT_SUCCESS });
}

/* * * * * * * * * * * * * * * * * * * * */