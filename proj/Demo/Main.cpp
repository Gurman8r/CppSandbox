/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include "DemoSettings.hpp"
#include "Testing.hpp"

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
	if (auto program = static_cast<ml::Window *>(new DEMO::Demo()))
	{
		// Enter
		ML_EventSystem.fireEvent(ml::EnterEvent(argc, argv));
		if (ml::Debug::checkError(ML_FAILURE))
		{
			delete program;
			return ml::Debug::logError("Failed Entering Program")
				|| ml::Debug::pause(EXIT_FAILURE);
		}

		// Load
		ML_EventSystem.fireEvent(ml::LoadEvent());

		// Start
		ML_EventSystem.fireEvent(ml::StartEvent());

		// Loop
		ml::Timer	 timer;
		ml::Duration elapsed;
		do
		{	// Begin Step
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
			// End Step
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