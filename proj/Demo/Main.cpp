/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include "GUI.hpp"
#include <MemeCore/EventSystem.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Settings
	if (!SETTINGS.loadFromFile(CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	//return gui_main(argc, argv);

	// Create Demo
	DEMO::Demo demo;

	// Enter
	ML_EventSystem.fireEvent(DEMO::EnterEvent(argc, argv));
	if (demo.checkStatus() == ml::Debug::Error)
	{
		return ml::Debug::logError("Failed Entering Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
	
	// Load
	ML_EventSystem.fireEvent(DEMO::LoadEvent());
	if (demo.checkStatus() == ml::Debug::Error)
	{
		return ml::Debug::logError("Failed Loading Resources")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Start
	ML_EventSystem.fireEvent(DEMO::StartEvent());

	// Loop
	ml::Timer		loopTimer;
	ml::Duration	elapsed;
	ml::NativeInput	input;
	do
	{	// Begin Step
		loopTimer.start();
		input.beginStep();
		{
			// Update
			ML_EventSystem.fireEvent(DEMO::UpdateEvent(elapsed, input));

			// Draw
			ML_EventSystem.fireEvent(DEMO::DrawEvent(elapsed));
		}
		// End Step
		input.endStep();
		elapsed = loopTimer.stop().elapsed();

	} while (demo.isOpen());

	// Exit
	ML_EventSystem.fireEvent(DEMO::ExitEvent());

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */