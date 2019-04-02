/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Sandbox.hpp"
#include "Settings.hpp"

#include <MemeCore/Debug.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeEngine/Engine.hpp>

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

	// Launch Program
	if (ml::Application * app = ML_Engine.launchApp(new DEMO::Sandbox()))
	{
		// Enter
		mlCheck(ML_EventSystem.fireEvent(ml::EnterEvent(argc, argv)));

		// Load Resources
		ML_EventSystem.fireEvent(ml::LoadEvent());

		// Start
		ML_EventSystem.fireEvent(ml::StartEvent());

		// Main Loop
		ml::Timer	 timer;
		ml::Duration elapsed;
		do
		{	// Begin Frame
			timer.start();
			app->pollEvents();
			{
				// Update Physics
				ML_EventSystem.fireEvent(ml::FixedUpdateEvent(elapsed));

				// Update Logic
				ML_EventSystem.fireEvent(ml::UpdateEvent(elapsed));

				// Draw Scene
				ML_EventSystem.fireEvent(ml::DrawEvent(elapsed));

				// Draw Gui
				ML_EventSystem.fireEvent(ml::GuiEvent(elapsed));
			}
			// End Frame
			app->swapBuffers();
			elapsed = timer.stop().elapsed();

		} while (app->isOpen());

		// Unload Resources
		ML_EventSystem.fireEvent(ml::UnloadEvent());

		// Exit
		ML_EventSystem.fireEvent(ml::ExitEvent());

		// Free Program
		return ML_Engine.freeApp();
	}
	else
	{
		return ml::Debug::logError("Failed Creating Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// WinMain
# if defined(ML_SYSTEM_WINDOWS)
int32_t __stdcall WinMain(void*, void*, char**, int32_t)
{
	return main(__argc, __argv);
}
# endif // ML_SYSTEM_WINDOWS

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */