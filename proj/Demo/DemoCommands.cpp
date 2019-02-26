#include "DemoCommands.hpp"
#include "Settings.hpp"
#include <MemeScript/Interpreter.hpp>
#include <MemeEditor/EditorConsole.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeEditor/ResourceManager.hpp>
#include <MemeCore/FileSystem.hpp>

namespace DEMO
{
	// Install
	/* * * * * * * * * * * * * * * * * * * * */

	void install_commands()
	{
		// Core
		ML_Interpreter.install({ "cat",		cmd_cat		});
		ML_Interpreter.install({ "cd",		cmd_cd		});
		ML_Interpreter.install({ "cwd",		cmd_cwd		});
		ML_Interpreter.install({ "exec",	cmd_exec	});
		ML_Interpreter.install({ "exists",	cmd_exists	});
		ML_Interpreter.install({ "exit",	cmd_exit	});
		ML_Interpreter.install({ "flag",	cmd_flag	});
		ML_Interpreter.install({ "get",		cmd_get		});
		ML_Interpreter.install({ "getcwd",	cmd_getcwd	});
		ML_Interpreter.install({ "help",	cmd_help	});
		ML_Interpreter.install({ "log",		cmd_log		});
		ML_Interpreter.install({ "ls",		cmd_ls		});
		ML_Interpreter.install({ "pause",	cmd_pause	});
		ML_Interpreter.install({ "read",	cmd_read	});
		ML_Interpreter.install({ "set",		cmd_set		});
		ML_Interpreter.install({ "system",	cmd_system	});
		ML_Interpreter.install({ "target",	cmd_target	});

		// Editor
		ML_Interpreter.install({ "clear",	cmd_clear	});
		ML_Interpreter.install({ "hide",	cmd_hide	});
		ML_Interpreter.install({ "history",	cmd_history });
		ML_Interpreter.install({ "list",	cmd_list	});
		ML_Interpreter.install({ "reload",	cmd_reload	});
		ML_Interpreter.install({ "run",		cmd_run		});
	}

	// Core
	/* * * * * * * * * * * * * * * * * * * * */
	
	ml::Var cmd_cat(ml::Args & args)
	{
		ml::String buf;
		if (ML_FileSystem.getFileContents(args.pop(), buf))
		{
			ml::cout << buf << ml::endl;

			return ml::Var().boolValue(true);
		}
		return ml::Var().boolValue(false);
	}
	
	ml::Var cmd_cd(ml::Args & args)
	{
		const ml::String path = args.pop();
		if (path.empty())
		{
			return ml::Var().boolValue(ML_FileSystem.setWorkingDir(SETTINGS.assetPath));
		}
		return ml::Var().boolValue(ML_FileSystem.setWorkingDir(path));
	}
	
	ml::Var cmd_cwd(ml::Args & args)
	{
		return ML_Interpreter.execCommand("getcwd").print();
	}
	
	ml::Var cmd_exec(ml::Args & args)
	{
		return ML_Interpreter.execFile(args.pop());
	}
	
	ml::Var cmd_exists(ml::Args & args)
	{
		const ml::String str = args.pop();

		if (ml::StringUtility::IsInt(str) && !args.empty())
		{
			return ml::Var().boolValue(
				ML_Runtime.getVar(
					ml::StringUtility::ToInt(str),
					args.pop())
			);
		}
		else if (str == "." || str == "..")
		{
			return ml::Var().boolValue(true);
		}
		else
		{
			return ml::Var().boolValue(ML_FileSystem.fileExists(str));
		}
	}
	
	ml::Var cmd_exit(ml::Args & args)
	{
		ML_EventSystem.fireEvent(ml::RequestExitEvent());
		return ml::Var().voidValue();
	}
	
	ml::Var cmd_flag(ml::Args & args)
	{
		if (!args.pop_front().empty())
		{
			const ml::String name = args.front();

			bool value;
			if (ml::StringUtility::MakeBool(args.pop(), value) && !args.empty())
			{
				if (name == "toks")
				{
					ML_Parser.showToks(value);
				}
				else if (name == "tree")
				{
					ML_Parser.showTree(value);
				}
				else if (name == "itop")
				{
					ML_Parser.showItoP(value);
				}
			}

			if (name == "toks")
			{
				return ml::Var().boolValue(ML_Parser.showToks());
			}
			else if (name == "tree")
			{
				return ml::Var().boolValue(ML_Parser.showTree());
			}
			else if (name == "itop")
			{
				return ml::Var().boolValue(ML_Parser.showItoP());
			}
			else
			{
				return ml::Var().errorValue("Flag Not Found {0}", name);
			}
		}
		return ml::Var().errorValue("No Flag Specified");
	}
	
	ml::Var cmd_get(ml::Args & args)
	{
		const ml::String scope = args.pop();

		if (ml::StringUtility::IsInt(scope) && !args.empty())
		{
			if (ml::Var * v = ML_Runtime.getVar(
				ml::StringUtility::ToInt(scope), args.pop()))
			{
				return (*v);
			}
		}
		return ml::Var().voidValue();
	}
	
	ml::Var cmd_getcwd(ml::Args & args)
	{
		return ml::Var().stringValue(ML_FileSystem.getWorkingDir());
	}
	
	ml::Var cmd_help(ml::Args & args)
	{
		ml::SStream ss;
		for (auto pair : ML_Interpreter.commands())
		{
			ml::cout << pair.first << ml::endl;
		}
		return ml::Var().boolValue(true);
	}
	
	ml::Var cmd_log(ml::Args & args)
	{
		const ml::String name = args.pop();
		if (name.empty())
		{
			return ml::Var().boolValue(false);
		}
		else if (name == "msg")
		{
			return ml::Var().intValue(ml::Debug::log(args.pop_front().str()));
		}
		else if (name == "wrn")
		{
			return ml::Var().intValue(ml::Debug::logWarning(args.pop_front().str()));
		}
		else if (name == "err")
		{
			return ml::Var().intValue(ml::Debug::logError(args.pop_front().str()));
		}
		else
		{
			return ml::Var().boolValue(false);
		}
	}
	
	ml::Var cmd_ls(ml::Args & args)
	{
		const ml::String name = args.pop_front().empty() ? "." : args.str();
		ml::SStream ss;
		if (ML_FileSystem.getDirContents(name, ss))
		{
			ml::cout << ss.str();
			return ml::Var().boolValue(true);
		}
		return ml::Var().boolValue(false);
	}
	
	ml::Var cmd_pause(ml::Args & args)
	{
		return ml::Var().intValue(ml::Debug::pause(EXIT_SUCCESS));
	}
	
	ml::Var cmd_read(ml::Args & args)
	{
		const ml::String name = args.pop();
		if (ML_FileSystem.fileExists(name))
		{
			ml::String buf;
			if (ML_FileSystem.getFileContents(name, buf))
			{
				return ml::Var().stringValue(buf);
			}
		}
		else
		{
			ml::SStream ss;
			if (ML_FileSystem.getDirContents(name, ss))
			{
				return ml::Var().stringValue(ss.str());
			}
		}
		return ml::Var().boolValue(false);
	}
	
	ml::Var cmd_set(ml::Args & args)
	{
		int32_t index;
		if (ml::StringUtility::MakeInt(args.pop(), index) && !args.empty())
		{
			const ml::String name = args.pop();

			if (ml::StringUtility::IsName(name) && !args.empty())
			{
				const ml::Token value = ML_Lexer.genToken(args.pop());

				if (ML_Runtime.setVar(
					index,
					name,
					ml::Var::makeSingle(value)
				))
				{
					return ml::Var().boolValue(true);
				}
			}
		}
		return ml::Var().boolValue(false);
	}
	
	ml::Var cmd_system(ml::Args & args)
	{
		return ml::Var().intValue(ml::Debug::system(args.pop_front().str().c_str()));
	}
	
	ml::Var cmd_target(ml::Args & args)
	{
		if (!args.pop_front().empty())
		{
			const ml::String & opt = args.front();
			if (opt == "name")
			{
				return ml::Var().stringValue(SETTINGS.title);
			}
			else if (opt == "config")
			{
				return ml::Var().stringValue(ml::Debug::configuration());
			}
			else if (opt == "platform")
			{
				return ml::Var().stringValue(ml::Debug::platform());
			}
		}
		return ml::Var().boolValue(true);
	}


	// Editor
	/* * * * * * * * * * * * * * * * * * * * */

	ml::Var cmd_clear(ml::Args & args)
	{
#ifdef ML_EditorConsole
		ML_EditorConsole.clear();
#endif
		return ml::Var().intValue(ml::Debug::clear());
	}

	ml::Var cmd_hide(ml::Args & args)
	{
#ifdef ML_EditorConsole
		ML_EditorConsole.visible() = false;
#endif
		return ml::Var().voidValue();
	}

	ml::Var cmd_history(ml::Args & args)
	{
#ifdef ML_EditorConsole
		ML_EditorConsole.printHistory();
#endif
		return ml::Var().boolValue(true);
	}

	ml::Var cmd_list(ml::Args & args)
	{
		const ml::String type = args.pop();
		if (type == "all")
		{
			ml::cout
				<< "# Fonts" << ml::endl << ML_Res.fonts
				<< "# Images" << ml::endl << ML_Res.images
				<< "# Materials" << ml::endl << ML_Res.mats
				<< "# Meshes" << ml::endl << ML_Res.meshes
				<< "# Models" << ml::endl << ML_Res.models
				<< "# Scripts" << ml::endl << ML_Res.scripts
				<< "# Shaders" << ml::endl << ML_Res.shaders
				<< "# Skyboxes" << ml::endl << ML_Res.skyboxes
				<< "# Sounds" << ml::endl << ML_Res.sounds
				<< "# Sprites" << ml::endl << ML_Res.sprites
				<< "# Textures" << ml::endl << ML_Res.textures
				<< ml::endl;
		}
		else if (type == "fonts") { ml::cout << ML_Res.fonts << ml::endl; }
		else if (type == "images") { ml::cout << ML_Res.images << ml::endl; }
		else if (type == "mats") { ml::cout << ML_Res.mats << ml::endl; }
		else if (type == "meshes") { ml::cout << ML_Res.meshes << ml::endl; }
		else if (type == "models") { ml::cout << ML_Res.models << ml::endl; }
		else if (type == "scripts") { ml::cout << ML_Res.scripts << ml::endl; }
		else if (type == "shaders") { ml::cout << ML_Res.shaders << ml::endl; }
		else if (type == "skyboxes") { ml::cout << ML_Res.skyboxes << ml::endl; }
		else if (type == "sounds") { ml::cout << ML_Res.sounds << ml::endl; }
		else if (type == "sprites") { ml::cout << ML_Res.sprites << ml::endl; }
		else if (type == "textures") { ml::cout << ML_Res.textures << ml::endl; }
		else
		{
			return ml::Var().errorValue("Type not found: {0}", type);
		}
		return ml::Var().boolValue(true);
	}

	ml::Var cmd_reload(ml::Args & args)
	{
		const ml::String type = args.pop();
		if (type == "all")
		{
			return ml::Var().intValue((int32_t)ML_Res.reloadAll());
		}
		else if (type == "fonts") { return ml::Var().intValue((int32_t)ML_Res.fonts.reload()); }
		else if (type == "images") { return ml::Var().intValue((int32_t)ML_Res.images.reload()); }
		else if (type == "mats") { return ml::Var().intValue((int32_t)ML_Res.mats.reload()); }
		else if (type == "meshes") { return ml::Var().intValue((int32_t)ML_Res.meshes.reload()); }
		else if (type == "models") { return ml::Var().intValue((int32_t)ML_Res.models.reload()); }
		else if (type == "scripts") { return ml::Var().intValue((int32_t)ML_Res.scripts.reload()); }
		else if (type == "shaders") { return ml::Var().intValue((int32_t)ML_Res.shaders.reload()); }
		else if (type == "skyboxes") { return ml::Var().intValue((int32_t)ML_Res.skyboxes.reload()); }
		else if (type == "sounds") { return ml::Var().intValue((int32_t)ML_Res.sounds.reload()); }
		else if (type == "sprites") { return ml::Var().intValue((int32_t)ML_Res.sprites.reload()); }
		else if (type == "textures") { return ml::Var().intValue((int32_t)ML_Res.textures.reload()); }
		else
		{
			return ml::Var().errorValue("Type not found: {0}", type);
		}
	}

	ml::Var cmd_run(ml::Args & args)
	{
		bool flag_rebuild = args.find_and_erase("-r");

		const ml::String name = args.pop();
		if (ml::Script * scr = ML_Res.scripts.get(name))
		{
			auto build_fun = flag_rebuild
				? &ml::Script::rebuild
				: &ml::Script::build;

			args.pop_front();

			if ((scr->*build_fun)(args) && scr->run())
			{
				return scr->out();
			}
		}
		return ml::Var().errorValue("Script not found: {0}", name);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}