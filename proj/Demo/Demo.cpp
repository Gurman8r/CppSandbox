/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv)
{
	// Start master timer
	ML_Time.start();
	ml::Debug::Log("{0}", ML_Time.elapsed());

	// Load Settings
	if (!demo::settings.load(CONFIG_INI))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Run Boot Script
	ml::Interpreter::LoadBuiltinCommands();
	(*ML_Interpreter.parser())
		.showToks(demo::settings.showToks)
		.showTree(demo::settings.showTree)
		.showItoP(demo::settings.showItoP);
	ML_Interpreter.execScript(demo::settings.pathTo(demo::settings.script));

	// Enable GL Error Pause
	ml::OpenGL::errorPause(true);

	// Create Window
	ml::Debug::Log("Creating Window...");
	ml::RenderWindow window;
	if (!window.create(
		demo::settings.title,
		ml::VideoMode({ demo::settings.width, demo::settings.height }, 32),
		ml::Window::Style::Default,
		ml::Context(3, 3, 24, 8, ml::Context::Compat, false, false)))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Load Assets
	if (!demo::loadAssets())
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Initialize
	if (!demo::init({ window }))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	ml::Debug::Log("{0}", ML_Time.elapsed());

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
			demo::update({ window, elapsed, input });

			// Draw
			demo::draw({ window, elapsed });
		}
		// End Step
		input.endStep();
		elapsed = loopTimer.stop().elapsed();
	}
	while (window.isOpen());

	ml::Debug::Log("{0}", ML_Time.elapsed());

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */