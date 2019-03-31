#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <MemeEditor/MainMenuBar.hpp>
#include <MemeEditor/Terminal.hpp>
#include <MemeEditor/Builder.hpp>
#include <MemeEditor/Browser.hpp>
#include <MemeEditor/ImGui_Builtin.hpp>
#include <MemeEditor/Dockspace.hpp>
#include <MemeEditor/TextEditor.hpp>
#include <MemeEditor/Hierarchy.hpp>
#include <MemeEditor/SceneView.hpp>
#include <MemeEditor/Inspector.hpp>
#include <MemeEditor/ResourceHUD.hpp>
#include <MemeEditor/NetworkHUD.hpp>
#include <MemeEditor/Profiler.hpp>

#define ML_Editor ml::Editor::getInstance()

namespace ml
{
	class ML_EDITOR_API Editor final
		: public ITrackable
		, public ISingleton<Editor>
	{
		friend class ISingleton<Editor>;

	private:
		Editor();
		~Editor();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool show_mainMenuBar	= true;
		bool show_dockspace		= true;
		/* * * * * * * * * * * * * * * * * * * * */
		bool show_imgui_demo	= false;
		bool show_imgui_metrics	= false;
		bool show_imgui_style	= false;
		bool show_imgui_about	= false;
		/* * * * * * * * * * * * * * * * * * * * */
		bool show_terminal		= true;
		bool show_browser		= true;
		bool show_builder		= true;
		bool show_inspector		= true;
		bool show_sceneView		= true;
		bool show_textEditor	= true;
		bool show_hierarchy		= true;
		bool show_resources		= true;
		bool show_profiler		= true;
		bool show_network		= false;
		/* * * * * * * * * * * * * * * * * * * * */
	};

	
}

#endif // !_ML_EDITOR_HPP_