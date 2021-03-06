#include <MemeEditor/MainMenuBar.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEditor/Editor.hpp>
#include <MemeEditor/EditorEvents.hpp>
#include <MemeEngine/EngineEvents.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/CoreEvents.hpp>
#include <MemeCore/OS.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	MainMenuBar::MainMenuBar()
		: GUI_Tool()
	{
		ML_OS; // initialize OS singleton
	}

	MainMenuBar::~MainMenuBar()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool MainMenuBar::beginDraw()
	{
		return goodCheck(ImGui::BeginMainMenuBar());
	}

	bool MainMenuBar::endDraw()
	{
		ImGui::EndMainMenuBar();
		return good();
	}

	bool MainMenuBar::drawGui()
	{
		if (beginDraw())
		{
			// File Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					ML_EventSystem.fireEvent(File_New_Event());
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					ML_EventSystem.fireEvent(File_Open_Event());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					ML_EventSystem.fireEvent(File_Save_Event(false));
				}
				if (ImGui::MenuItem("Save All", "Ctrl+Shift+S", false))
				{
					ML_EventSystem.fireEvent(File_Save_Event(true));
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					ML_EventSystem.fireEvent(File_Close_Event());
				}
				ImGui::EndMenu();
			}

			// Edit Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z"))
				{
					ML_EventSystem.fireEvent(Edit_Undo_Event());
				}
				if (ImGui::MenuItem("Redo", "Ctrl+Y"))
				{
					ML_EventSystem.fireEvent(Edit_Redo_Event());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X"))
				{
					ML_EventSystem.fireEvent(Edit_Cut_Event());
				}
				if (ImGui::MenuItem("Copy", "Ctrl+C"))
				{
					ML_EventSystem.fireEvent(Edit_Copy_Event());
				}
				if (ImGui::MenuItem("Paste", "Ctrl+V"))
				{
					ML_EventSystem.fireEvent(Edit_Paste_Event());
				}
				ImGui::EndMenu();
			}

			// Window Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem(ML_Terminal.title(), "Ctrl+Alt+T", &ML_Editor.show_terminal);
				ImGui::MenuItem(ML_Browser.title(), "Ctrl+Alt+E", &ML_Editor.show_browser);
				ImGui::MenuItem(ML_Builder.title(), "Ctrl+Alt+B", &ML_Editor.show_builder);
				ImGui::MenuItem(ML_SceneView.title(), "Ctrl+Alt+S", &ML_Editor.show_sceneView);
				ImGui::MenuItem(ML_Inspector.title(), "Ctrl+Alt+I", &ML_Editor.show_inspector);
				ImGui::MenuItem(ML_Profiler.title(), NULL, &ML_Editor.show_profiler);
				ImGui::MenuItem(ML_TextEditor.title(), NULL, &ML_Editor.show_textEditor);
				ImGui::MenuItem(ML_ResourceView.title(), NULL, &ML_Editor.show_resourceView);
				ImGui::MenuItem(ML_NetworkHUD.title(), NULL, &ML_Editor.show_network);
				ImGui::EndMenu();
			}

			// Help Menu
			/* * * * * * * * * * * * * * * * * * * * */
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Project Page"))
				{
					ML_EventSystem.fireEvent(OS_ExecuteEvent("open", ML_PROJECT_URL));
				}
				ImGui::Separator();
				ImGui::MenuItem("ImGui Demo", "Ctrl+H", &ML_Editor.show_imgui_demo);
				ImGui::MenuItem("ImGui Metrics", NULL, &ML_Editor.show_imgui_metrics);
				ImGui::MenuItem("ImGui Style Editor", NULL, &ML_Editor.show_imgui_style);
				ImGui::MenuItem("About Dear ImGui", NULL, &ML_Editor.show_imgui_about);
				ImGui::EndMenu();
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}