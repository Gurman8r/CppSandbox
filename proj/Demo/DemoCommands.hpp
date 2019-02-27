#ifndef _DEMO_COMMANDS_HPP_
#define _DEMO_COMMANDS_HPP_

#include <MemeScript/Args.hpp>
#include <MemeScript/Var.hpp>

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	void install_commands();

	// Core
	/* * * * * * * * * * * * * * * * * * * * */
	static ml::Var cmd_cat		(ml::Args & args); // Display File
	static ml::Var cmd_cd		(ml::Args & args); // Change Directory
	static ml::Var cmd_config	(ml::Args & args); // Read/Modify Settings
	static ml::Var cmd_cwd		(ml::Args & args); // Print Working Directory
	static ml::Var cmd_exec		(ml::Args & args); // Execute File
	static ml::Var cmd_exists	(ml::Args & args); // Check Exists (File/Var)
	static ml::Var cmd_exit		(ml::Args & args); // Fire RequestExitEvent
	static ml::Var cmd_get		(ml::Args & args); // Get Variable
	static ml::Var cmd_getcwd	(ml::Args & args); // Return Working Directory
	static ml::Var cmd_help		(ml::Args & args); // Display Installed Commands
	static ml::Var cmd_log		(ml::Args & args); // Debug Log (msg/wrn/err)
	static ml::Var cmd_ls		(ml::Args & args); // Display a Directory
	static ml::Var cmd_pause	(ml::Args & args); // System Pause
	static ml::Var cmd_read		(ml::Args & args); // Return File/Dir Contents
	static ml::Var cmd_set		(ml::Args & args); // Set Variable
	static ml::Var cmd_system	(ml::Args & args); // System Call
	static ml::Var cmd_target	(ml::Args & args); // Return Program Info
	
	// Editor
	/* * * * * * * * * * * * * * * * * * * * */
	static ml::Var cmd_clear	(ml::Args & args); // Clear Editor Console
	static ml::Var cmd_hide		(ml::Args & args); // Hide Editor Console
	static ml::Var cmd_history	(ml::Args & args); // Display History
	static ml::Var cmd_list		(ml::Args & args); // Display Resources
	static ml::Var cmd_reload	(ml::Args & args); // Reload Resources
	static ml::Var cmd_run		(ml::Args & args); // Run Loaded Script

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_DEMO_COMMANDS_HPP_
