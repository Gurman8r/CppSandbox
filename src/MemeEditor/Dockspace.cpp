#include <MemeEditor/Dockspace.hpp>
#include <MemeEditor/Inspector.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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
		, border		(0.0f)
		, fullscreen	(true)
		, padding		(vec2f::Zero)
		, rounding		(0.0f)
		, size			(vec2f::Zero)
		, title			(title)
		, win_flags		(ImGuiWindowFlags_None)
		, bgAlpha		(1.0f)
	{
	}

	Dockspace::Dockspace(const Dockspace & copy)
		: dock_flags	(copy.dock_flags)
		, border		(copy.border)
		, fullscreen	(copy.fullscreen)
		, padding		(copy.padding)
		, rounding		(copy.rounding)
		, size			(copy.size)
		, title			(copy.title)
		, win_flags		(copy.win_flags)
		, bgAlpha		(copy.bgAlpha)
	{
	}

	Dockspace::~Dockspace() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Dockspace::begin_dock(bool * p_open)
	{
		if (m_good = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
		{
			// Flags
			win_flags =
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse;

			if (fullscreen)
			{
				win_flags |=
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus |
					ImGuiWindowFlags_MenuBar |
					ImGuiWindowFlags_NoDocking;
			}

			if (dock_flags & ImGuiDockNodeFlags_PassthruDockspace)
			{
				win_flags |= ImGuiWindowFlags_NoBackground;
			}

			// Bounds
			const ImGuiViewport * viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			// Style
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, border);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { padding[0], padding[1] });
			ImGui::SetNextWindowBgAlpha(bgAlpha);

			// Begin
			if (m_good = (ImGui::Begin(title.c_str(), p_open, win_flags)))
			{
				ImGui::PopStyleVar(3);
			}
		}
		return m_good;
	}

	void Dockspace::end_dock()
	{
		if (m_good)
		{
			ImGui::DockSpace(getID(), { size[0], size[1] }, dock_flags);
		}

		ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t Dockspace::getID() const
	{
		return ImGui::GetID(getTitle());
	}

	CString Dockspace::getTitle() const
	{
		return title.c_str();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}