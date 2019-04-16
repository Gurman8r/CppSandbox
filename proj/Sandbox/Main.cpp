/* * * * * * * * * * * * * * * * * * * * */

#include "Sandbox.hpp"
#include "Settings.hpp"

#include <MemeCore/Debug.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeEngine/Engine.hpp>
#include <MemeEngine/StateMachine.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifndef ML_CONFIG_INI
# define ML_CONFIG_INI "../../../ML_Config.ini"
# endif

/* * * * * * * * * * * * * * * * * * * * */

int32_t main(int32_t argc, char ** argv)
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Load Settings
	if (!SETTINGS.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings: \'{0}\'", ML_CONFIG_INI)
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Setup State Machine

	enum State : int32_t
	{
		None = ML_STATE_INVALID,
		Enter, Load, Start, Loop, Unload, Exit,
		MAX_STATE
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine sm =
	{
	{ State::Enter, [](void * data)
	{
		mlCheck(ML_EventSystem.fireEvent(ml::EnterEvent(
			__argc, __argv
		)));
		return sm.run(State::Load, data);
	}},
	{ State::Load, [](void * data)
	{
		ML_EventSystem.fireEvent(ml::LoadEvent());
		return sm.run(State::Start, data);
	} },
	{ State::Start, [](void * data)
	{
		ML_EventSystem.fireEvent(ml::StartEvent());
		return sm.run(State::Loop, data);
	} },
	{ State::Loop, [](void * data)
	{
		ML_Engine.loop([]()
		{
			// Update
			ML_EventSystem.fireEvent(ml::UpdateEvent(
				ML_Engine.elapsed()
			));

			// Draw
			ML_EventSystem.fireEvent(ml::DrawEvent(
				ML_Engine.elapsed()
			));

			// Gui
			ML_EventSystem.fireEvent(ml::GuiEvent(
				ML_Engine.elapsed()
			));
		});
		return sm.run(State::Unload, data);
	} },
	{ State::Unload, [](void * data)
	{
		ML_EventSystem.fireEvent(ml::UnloadEvent());
		return sm.run(State::Exit, data);
	} },
	{ State::Exit, [](void * data)
	{
		ML_EventSystem.fireEvent(ml::ExitEvent());
		return sm.run(State::None, data);
	} },
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Launch Application
	if (auto * app = ML_Engine.launchApp(new DEMO::Sandbox()))
	{
		// Run State Machine
		sm.run(State::Enter, 0);

		// Free Application
		return ML_Engine.freeApp(app);
	}
	else
	{
		// Something went wrong...
		return ml::Debug::logError("Failed Launching Application")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}