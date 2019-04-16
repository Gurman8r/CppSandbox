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
	/* * * * * * * * * * * * * * * * * * * * */
	if (!SETTINGS.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * */
	if (auto * app = ML_Engine.launchApp(new DEMO::Sandbox()))
	{
		// Enter
		mlCheck(ML_EventSystem.fireEvent(ml::EnterEvent(argc, argv)));

		// Load Resources
		ML_EventSystem.fireEvent(ml::LoadEvent());

		// Start
		ML_EventSystem.fireEvent(ml::StartEvent());

		/* * * * * * * * * * * * * * * * * * * * */

		// Main Loop
		ML_Engine.loop([&]() 
		{
			// Update Logic
			ML_EventSystem.fireEvent(ml::UpdateEvent(
				ML_Engine.elapsed()
			));

			// Draw Scene
			ML_EventSystem.fireEvent(ml::DrawEvent(
				ML_Engine.elapsed()
			));

			// Draw Gui
			ML_EventSystem.fireEvent(ml::GuiEvent(
				ML_Engine.elapsed()
			));
		});

		/* * * * * * * * * * * * * * * * * * * * */

		// Unload Resources
		ML_EventSystem.fireEvent(ml::UnloadEvent());

		// Exit
		ML_EventSystem.fireEvent(ml::ExitEvent());

		// Free Application
		return ML_Engine.freeApp(app);
	}
	else
	{
		return ml::Debug::logError("Failed Creating Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SYSTEM_WINDOWS)
int32_t __stdcall WinMain(void*, void*, char**, int32_t)
{
	return main(__argc, __argv);
}
# endif // ML_SYSTEM_WINDOWS

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */