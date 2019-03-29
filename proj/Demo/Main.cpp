/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include "Testing.hpp"
#include <MemeCore/Debug.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/Dispatcher.hpp>
#include <MemeCore/TestRequest.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ML_CONFIG_INI
#define ML_CONFIG_INI "../../../ML_Config.ini"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



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
	ml::Window * program = static_cast<ml::Window *>(new DEMO::Demo());
	
	// Enter
	ML_EventSystem.fireEvent(DEMO::EnterEvent(ml::Args(argc, argv)));
	if (ml::Debug::checkError(ML_FAILURE))
	{
		delete program;
		return ml::Debug::logError("Failed Entering Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Load
	ML_EventSystem.fireEvent(DEMO::LoadEvent());

	// Start
	ML_EventSystem.fireEvent(DEMO::StartEvent());

	// Loop
	ml::Timer	 timer;
	ml::Duration elapsed;
	do
	{	// Begin Step
		timer.start();
		{
			// "Fixed" Update
			ML_EventSystem.fireEvent(DEMO::FixedUpdateEvent(elapsed));

			// Update
			ML_EventSystem.fireEvent(DEMO::UpdateEvent(elapsed));

			// Draw
			ML_EventSystem.fireEvent(DEMO::DrawEvent(elapsed));
		}
		// End Step
		elapsed = timer.stop().elapsed();

	} while (program->isOpen());

	// Unload
	ML_EventSystem.fireEvent(DEMO::UnloadEvent());

	// Exit
	ML_EventSystem.fireEvent(DEMO::ExitEvent());

	// Delete Program
	delete program;

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */