/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include <MemeCore/Debug.hpp>
#include <MemeCore/Timer.hpp>
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
	ml::RenderWindow * program;
	if (!(program = static_cast<ml::RenderWindow *>(new DEMO::DemoProgram())))
	{
		delete program;
		return ml::Debug::logError("Failed Creating Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
	else
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

		// Exit
		ML_EventSystem.fireEvent(DEMO::ExitEvent());

		// Delete Program
		delete program;
		return EXIT_SUCCESS;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */