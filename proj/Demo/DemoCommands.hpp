#ifndef _DEMO_COMMANDS_HPP_
#define _DEMO_COMMANDS_HPP_

#include <MemeScript/Args.hpp>
#include <MemeScript/Var.hpp>

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	void install_demo_commands();

	/* * * * * * * * * * * * * * * * * * * * */

	static ml::Var cmd_help		(ml::Args & args);
	static ml::Var cmd_history	(ml::Args & args);
	static ml::Var cmd_system	(ml::Args & args);
	static ml::Var cmd_log		(ml::Args & args);
	static ml::Var cmd_exit		(ml::Args & args);
	static ml::Var cmd_hide		(ml::Args & args);
	static ml::Var cmd_clear	(ml::Args & args);
	static ml::Var cmd_run		(ml::Args & args);
	static ml::Var cmd_getcwd	(ml::Args & args);
	static ml::Var cmd_cwd		(ml::Args & args);
	static ml::Var cmd_pause	(ml::Args & args);
	static ml::Var cmd_cd		(ml::Args & args);
	static ml::Var cmd_cat		(ml::Args & args);
	static ml::Var cmd_read		(ml::Args & args);
	static ml::Var cmd_get		(ml::Args & args);
	static ml::Var cmd_set		(ml::Args & args);
	static ml::Var cmd_exists	(ml::Args & args);
	static ml::Var cmd_exec		(ml::Args & args);
	static ml::Var cmd_ls		(ml::Args & args);
	static ml::Var cmd_flag		(ml::Args & args);
	static ml::Var cmd_target	(ml::Args & args);
	static ml::Var cmd_list		(ml::Args & args);
	static ml::Var cmd_reload	(ml::Args & args);

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_DEMO_COMMANDS_HPP_
