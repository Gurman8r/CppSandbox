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
	// Load Settings
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (!SETTINGS.loadFromFile(ML_CONFIG_INI))
	{
		return ml::Debug::logError("Failed Loading Settings: \'{0}\'", ML_CONFIG_INI)
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	// Setup Control States
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum State : int32_t
	{
		None = ML_STATE_INVALID,
		Enter, Load, Start, Loop, Unload, Exit,
		MAX_STATE
	};

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::StateMachine<State, int32_t, void *> control =
	{
	{ State::Enter, [](auto data)
	{
		// Enter
		ML_EventSystem.fireEvent(ml::EnterEvent(__argc, __argv));
		return control.run(State::Load, data);
	} },
	{ State::Load, [](auto data)
	{
		// Load
		ML_EventSystem.fireEvent(ml::LoadEvent());
		return control.run(State::Start, data);
	} },
	{ State::Start, [](auto data)
	{
		// Start
		ML_EventSystem.fireEvent(ml::StartEvent());
		return control.run(State::Loop, data);
	} },
	{ State::Loop, [](auto data)
	{
		// Loop
		ML_Engine.loop([]()
		{
			// Update
			ML_EventSystem.fireEvent(ml::UpdateEvent(ML_Engine.frameTime()));

			// Draw
			ML_EventSystem.fireEvent(ml::DrawEvent(ML_Engine.frameTime()));

			// Gui
			ML_EventSystem.fireEvent(ml::GuiEvent(ML_Engine.frameTime()));
		});
		return control.run(State::Unload, data);
	} },
	{ State::Unload, [](auto data)
	{
		// Unload
		ML_EventSystem.fireEvent(ml::UnloadEvent());
		return control.run(State::Exit, data);
	} },
	{ State::Exit, [](auto data)
	{
		// Exit
		ML_EventSystem.fireEvent(ml::ExitEvent());
		return control.run(State::None, data);
	} },
	};

	// Launch Application
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (ml::Application * app = ML_Engine.launchApp(new DEMO::Sandbox()))
	{
		control.run(State::Enter, NULL);

		return ML_Engine.freeApp(app);
	}
	else
	{
		return ml::Debug::logError("Failed Launching Application")
			|| ml::Debug::pause(EXIT_FAILURE);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}