/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include <MemeCore/Debug.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/LibLoader.hpp>

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

	// Load Plugins
	if (void * lib = ML_Lib.loadPlugin(ml::String("./TestPlugin_{0}_{1}.dll").format(
		ml::Debug::configuration(),
		ml::Debug::platform()),
		(void *)("Data Received by Plugin")))
	{
		ML_Lib.freeLibrary(lib);
	}
	else
	{
		return ml::Debug::logError("Failed Loading Plugins")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Load Program
	if (auto * program = new DEMO::Demo())
	{
		// Enter
		ML_EventSystem.fireEvent(DEMO::EnterEvent(ml::Args(argc, argv)));
		if (ml::Debug::checkError(ML_FAILURE))
		{
			delete program;
			return ml::Debug::pause(EXIT_FAILURE);
		}

		// Load
		ML_EventSystem.fireEvent(DEMO::LoadEvent());
		if (ml::Debug::checkError(ML_FAILURE))
		{
			delete program;
			return ml::Debug::pause(EXIT_FAILURE);
		}

		// Start
		ML_EventSystem.fireEvent(DEMO::StartEvent());
		if (ml::Debug::checkError(ML_FAILURE))
		{
			delete program;
			return ml::Debug::pause(EXIT_FAILURE);
		}

		// Loop
		ml::Timer	 loopTimer;
		ml::Duration elapsed;
		do
		{	// Begin Step
			loopTimer.start();
			{
				// "Fixed" Update
				ML_EventSystem.fireEvent(DEMO::FixedUpdateEvent(elapsed));
				// Update
				ML_EventSystem.fireEvent(DEMO::UpdateEvent(elapsed));
				// Draw
				ML_EventSystem.fireEvent(DEMO::DrawEvent(elapsed));
			}
			// End Step
			elapsed = loopTimer.stop().elapsed();

		} while (program->isOpen());

		// Exit
		ML_EventSystem.fireEvent(DEMO::ExitEvent());
		
		// Free Program
		delete program;

		return EXIT_SUCCESS;
	}
	else
	{
		return ml::Debug::logError("Failed Creating Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
}

/* * * * * * * * * * * * * * * * * * * * */