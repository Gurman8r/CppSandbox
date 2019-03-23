#include <MemeEditor/Dockspace.hpp>
#include <MemeEditor/EditorGUI.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Dockspace::Dockspace()
		: GUIDrawer()
		, dock_flags(0)
		, border	(0.0f)
		, fullscreen(true)
		, padding	(vec2f::Zero)
		, rounding	(0.0f)
		, size		(vec2f::Zero)
		, bgAlpha	(1.0f)
	{
	}

	Dockspace::~Dockspace() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Dockspace::beginDraw(CString title, bool * p_open, int32_t flags)
	{
		if (m_good = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
		{
			// Flags
			ImGuiWindowFlags window_flags =
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse;

			if (fullscreen)
			{
				window_flags |=
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus |
					ImGuiWindowFlags_MenuBar |
					ImGuiWindowFlags_NoDocking;
			}

			if (dock_flags & ImGuiDockNodeFlags_PassthruDockspace)
			{
				window_flags |= ImGuiWindowFlags_NoBackground;
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
			if (GUIDrawer::beginDraw(title, p_open, window_flags))
			{
				ImGui::PopStyleVar(3);
			}
		}
		return m_good;
	}

	bool Dockspace::endDraw()
	{
		if (m_good)
		{
			ImGui::DockSpace(getID(), { size[0], size[1] }, dock_flags);
		}
		return GUIDrawer::endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t Dockspace::split(uint32_t dir, float ratio, uint32_t * out, uint32_t * other)
	{
		return split(getID(), dir, ratio, out, other);
	}

	uint32_t Dockspace::split(uint32_t id, int32_t dir, float ratio, uint32_t * other)
	{
		return split(id, dir, ratio, NULL, other);
	}

	uint32_t Dockspace::split(uint32_t id, int32_t dir, float ratio, uint32_t * out, uint32_t * other)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, other);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Dockspace::dock_window(CString name, uint32_t id)
	{
		return ImGui::DockBuilderDockWindow(name, id);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}