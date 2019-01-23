/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"

using namespace demo;

/* * * * * * * * * * * * * * * * * * * * */

int main(int argc, char** argv)
{
	// Start master timer
	ML_Time.start();

	ml::Debug::Log("{0}", ML_Time.elapsed());

	// Load Settings
	if (!settings.load(CONFIG_INI))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Run Boot Script
	ml::Interpreter::LoadBuiltinCommands();
	(*ML_Interpreter.parser())
		.showToks(settings.showToks)
		.showTree(settings.showTree)
		.showItoP(settings.showItoP);
	ML_Interpreter.execScript(settings.pathTo(settings.bootScript));

	// Enable GL Error Pause
	ml::OpenGL::errorPause(true);

	// Create Window
	ml::Debug::Log("Creating Window...");
	ml::RenderWindow window;
	if (!window.create(
		settings.title,
		ml::VideoMode({ settings.width, settings.height }, 32),
		ml::Window::Style::Default,
		ml::Context(3, 3, 24, 8, ml::Context::Compat, false, false)))
	{
		return ml::Debug::pause(EXIT_FAILURE);
	}

	// Loading / Setup
	if (!loadAssets() || !demo_setup({ window }))
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
			demo_update({ window, elapsed, input });

			// Draw
			demo_draw({ window, elapsed });
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