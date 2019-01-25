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
		return ml::Debug::LogError("Failed Loading Settings")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Program Enter
	if (!demo::onProgramEnter({ argc, argv }))
	{
		return ml::Debug::LogError("Failed Entering Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Create Window
	ml::RenderWindow window;
	if (ml::Debug::Log("Creating Window..."))
	{
		if (!window.create(
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
				SETTINGS.sRgbCapable)
		))
		{
			return ml::Debug::LogError("Failed Creating Window")
				|| ml::Debug::pause(EXIT_FAILURE);
		}

		window.setCursor(ml::Window::Cursor::Normal);
		window.setPosition((ml::VideoMode::desktop().size - window.size()) / 2);
		window.setViewport(ml::vec2i::Zero, window.size());
	}

	// Load Resources
	if (!demo::onLoadResources({ }))
	{
		return ml::Debug::LogError("Failed Loading Resources")
			|| ml::Debug::pause(EXIT_FAILURE);
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