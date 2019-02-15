/* * * * * * * * * * * * * * * * * * * * */

#include "Demo.hpp"
#include <MemeCore/EventSystem.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define CONFIG_INI "../../../config.ini"

/* * * * * * * * * * * * * * * * * * * * */

struct Test
{
	void test1() {}
	void test2(int i) {}
};

inline static int32_t testThreads()
{
	bool finished = false;
	ml::Thread t1([&]() 
	{
		using namespace ml;
		ml::cout << "Working..." << ml::endl;
		while (!finished)
		{
			ml::cout << "*";
			ML_Time.sleep(1_s);
		}
		ml::cout << ml::endl;
	});
	t1.launch();
	
	ml::cin.get();
	finished = true;
	
	t1.join();

	return ml::Debug::pause(EXIT_SUCCESS);
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

	// Threads
	//return testThreads();

	// Create Demo
	DEMO::Demo * demo = new DEMO::Demo();

	// Enter
	ML_EventSystem.fireEvent(DEMO::EnterEvent(argc, argv));
	if (demo->getError() == ml::Debug::Error)
	{
		return ml::Debug::logError("Failed Entering Program")
			|| ml::Debug::pause(EXIT_FAILURE);
	}
	
	// Load
	ML_EventSystem.fireEvent(DEMO::LoadEvent());
	if (demo->getError() == ml::Debug::Error)
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
			ML_EventSystem.fireEvent(DEMO::UpdateEvent(elapsed));

			// Draw
			ML_EventSystem.fireEvent(DEMO::DrawEvent(elapsed));
		}
		// End Step
		input.endStep();
		elapsed = loopTimer.stop().elapsed();

	} while (demo->isOpen());

	// Exit
	ML_EventSystem.fireEvent(DEMO::ExitEvent());

	delete demo;

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */