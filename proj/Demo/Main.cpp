/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include <MemeCore/Debug.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/LibLoader.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */

ml::RenderWindow * exampleProgramLoader(const ml::String & filename)
{
	if (void * lib = ML_Lib.loadLibrary(filename))
	{
		if (void * fun = ML_Lib.loadFunction(lib, "ML_LoadInstance"))
		{
			if (void * ent = ML_Lib.callFunction<void *>(fun))
			{
				return static_cast<ml::RenderWindow *>(ent);
			}
		}
		ML_Lib.freeLibrary(lib);
		return NULL;
	}
	return static_cast<ml::RenderWindow *>(new DEMO::Demo());
}

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	// Load Settings
	if (!SETTINGS.loadFromFile(CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Load Program
	if (ml::RenderWindow * program = exampleProgramLoader("./Placeholder_Debug_x86.dll"))
	{
		// Enter
		ML_EventSystem.fireEvent(DEMO::EnterEvent(argc, argv));
		if (ml::Debug::checkError(ML_FAILURE))
		{
			delete program;
			return ml::Debug::logError("Failed Entering Program")
				|| ml::Debug::pause(EXIT_FAILURE);
		}

		// Load
		ML_EventSystem.fireEvent(DEMO::LoadEvent());
		if (ml::Debug::checkError(ML_FAILURE))
		{
			delete program;
			return ml::Debug::logError("Failed Loading Resources")
				|| ml::Debug::pause(EXIT_FAILURE);
		}

		// Start
		ML_EventSystem.fireEvent(DEMO::StartEvent());

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