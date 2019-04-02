/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Sandbox.hpp"
#include "Settings.hpp"

#include <MemeCore/Debug.hpp>
#include <MemeCore/EventSystem.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CONFIG_INI
#define ML_CONFIG_INI "../../../ML_Config.ini"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Settings
	if (!SETTINGS.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Create Program
	if (auto program = new DEMO::Sandbox())
	{
		// Enter
		mlCheck(ML_EventSystem.fireEvent(ml::EnterEvent(argc, argv)));

		// Load
		ML_EventSystem.fireEvent(ml::LoadEvent());

		// Start
		ML_EventSystem.fireEvent(ml::StartEvent());

		// Loop
		ml::Timer	 timer;
		ml::Duration elapsed;
		do
		{	// Begin Frame
			timer.start();
			program->pollEvents();
			{
				// "Fixed" Update
				ML_EventSystem.fireEvent(ml::FixedUpdateEvent(elapsed));

				// Update
				ML_EventSystem.fireEvent(ml::UpdateEvent(elapsed));

				// Draw
				ML_EventSystem.fireEvent(ml::DrawEvent(elapsed));

				// Gui
				ML_EventSystem.fireEvent(ml::GuiEvent(elapsed));
			}
			// End Frame
			program->swapBuffers();
			elapsed = timer.stop().elapsed();

		} while (program->isOpen());

		// Unload
		ML_EventSystem.fireEvent(ml::UnloadEvent());

		// Exit
		ML_EventSystem.fireEvent(ml::ExitEvent());

		// Delete Program
		delete program;

		return EXIT_SUCCESS;
	}
	else
	{
		return ml::Debug::logError("Failed Creating Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */