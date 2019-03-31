#ifndef _ML_EDITOR_COMMANDS_HPP_
#define _ML_EDITOR_COMMANDS_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeScript/Args.hpp>
#include <MemeScript/Var.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API EditorCommands
	{
		static void install();

		static Var cmd_clear	(Args & args); // Clear Inspector Console
		static Var cmd_history	(Args & args); // Display History
		static Var cmd_list		(Args & args); // Display Resources
		static Var cmd_reload	(Args & args); // Reload Resources
		static Var cmd_run		(Args & args); // Run Loaded Script
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_COMMANDS_HPP_