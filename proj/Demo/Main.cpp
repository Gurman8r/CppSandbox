/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.h"
#include "Testing.hpp"

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */


int32_t main(int32_t argc, char ** argv)
{
	// Create Demo
	demo::Demo game;
	
	// Load Settings
	if (!SETTINGS.loadFromFile(CONFIG_INI))
	{
		return ml::Debug::LogError("Failed Loading SendSettings")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Enter
	if (!game.onEnter({ argc, argv }))
	{
		return ml::Debug::LogError("Failed Entering Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Run Tests
	if (SETTINGS.runTests)
	{
		return tests::runTests(SETTINGS.runTests);
	}

	// Create Window
	ml::RenderWindow window;
	if (ml::Debug::Log("Creating Window..."))
	{
		if (!window.good() && window.create(
			SETTINGS.title,
			ml::VideoMode({ SETTINGS.width, SETTINGS.height }, SETTINGS.bitsPerPixel),
			ml::Window::Style::Default,
			ml::Context(
				SETTINGS.majorVersion,
				SETTINGS.minorVersion,
				SETTINGS.depthBits,
				SETTINGS.stencilBits,
				SETTINGS.profile,
				SETTINGS.multisample,
				SETTINGS.srgbCapable)
		))
		{
			window.setCursor(ml::Window::Cursor::Normal);
			window.setPosition((ml::VideoMode::desktop().size - window.size()) / 2);
			window.setViewport(ml::vec2i::Zero, window.size());
		}
		else
		{
			return ml::Debug::LogError("Failed Creating Window")
				|| ml::Debug::pause(EXIT_FAILURE);
		}
	}
	
	// Load
	if (!game.onLoad({ SETTINGS.logLoading }))
	{
		return ml::Debug::LogError("Failed Loading Resources")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Start
	game.onStart({ window });

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
			game.onUpdate({ window, elapsed, input });

			// Draw
			game.onDraw({ window, elapsed });
		}
		// End Step
		input.endStep();
		elapsed = loopTimer.stop().elapsed();

	} while (window.isOpen());

	// Exit
	return game.onExit({ EXIT_SUCCESS });
}

/* * * * * * * * * * * * * * * * * * * * */