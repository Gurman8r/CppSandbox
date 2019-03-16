#include <MemeEditor/Dockspace.hpp>
#include <MemeEditor/Inspector.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Dockspace::Dockspace()
		: Dockspace("Dockspace")
	{
	}

	Dockspace::Dockspace(const String & title)
		: Dockspace(title, ImGuiDockNodeFlags_PassthruDockspace)
	{
	}

	Dockspace::Dockspace(const String & title, int32_t dock_flags)
		: dock_flags	(dock_flags)
		, fullscreen	(true)
		, border		(0.0f)
		, padding		(vec2f::Zero)
		, rounding		(0.0f)
		, size			(vec2f::Zero)
		, title			(title)
		, win_flags		(ImGuiWindowFlags_None)
	{
	}

	Dockspace::Dockspace(const Dockspace & copy)
		: dock_flags	(copy.dock_flags)
		, fullscreen	(copy.fullscreen)
		, border		(copy.border)
		, padding		(copy.padding)
		, rounding		(copy.rounding)
		, size			(copy.size)
		, title			(copy.title)
		, win_flags		(copy.win_flags)
	{
	}

	Dockspace::~Dockspace() {}

	/* * * * * * * * * * * * * * * * * * * * */

	void Dockspace::update()
	{
		// We are using the ImGuiWindowFlags_NoDocking flag to make 
		// the parent window not dockable into, because it would be 
		// confusing to have two docking targets within each others.
		win_flags = (ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
		if (fullscreen)
		{
			ImGuiViewport * viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, border);

			win_flags |=
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove;

			win_flags |=
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, 
		// DockSpace() will render our background and handle the 
		// pass-thru hole, so we ask Begin() to not render a background.
		if (dock_flags & ImGuiDockNodeFlags_PassthruDockspace)
		{
			win_flags |= ImGuiWindowFlags_NoBackground;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { padding[0], padding[1] });
	}

	bool Dockspace::draw(const String & title, bool * p_open)
	{
		update();

		bool good;
		if (good = (ImGui::Begin((this->title = title).c_str(), p_open, win_flags)))
		{
			ImGui::PopStyleVar(fullscreen ? 3 : 1);

			if (good = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
			{
				ImGui::DockSpace(getID(), { size[0], size[1] }, dock_flags);
			}
		}
		return good;
	}

	uint32_t Dockspace::getID() const
	{
		return ImGui::GetID((title + "_ID").c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}