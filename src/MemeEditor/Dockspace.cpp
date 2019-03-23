#include <MemeEditor/Dockspace.hpp>
#include <MemeEditor/EditorGUI.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Dockspace::Dockspace()
		: GUI_Window()
		, border	(0.0f)
		, padding	(vec2f::Zero)
		, rounding	(0.0f)
		, size		(vec2f::Zero)
		, bgAlpha	(1.0f)
	{
	}

	Dockspace::~Dockspace() {}

	/* * * * * * * * * * * * * * * * * * * * */

	void Dockspace::onEvent(const IEvent * value)
	{
	}

	bool Dockspace::draw(CString title, bool * p_open)
	{
		return beginDraw(title, p_open, 
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBackground);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Dockspace::beginDraw(CString title, bool * p_open, int32_t flags)
	{
		if (m_good = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
		{
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
			if (GUI_Base::beginDraw(title, p_open, flags))
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
			ImGui::DockSpace(
				getID(),
				{ size[0], size[1] }, 
				ImGuiDockNodeFlags_PassthruDockspace
			);
		}
		return GUI_Base::endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t Dockspace::beginBuilder(int32_t flags)
	{
		if (uint32_t root = getID())
		{
			if (!ImGui::DockBuilderGetNode(root))
			{
				ImGui::DockBuilderRemoveNode(root);
				ImGui::DockBuilderAddNode(root, flags);
				return root;
			}
		}
		return 0;
	}

	uint32_t Dockspace::endBuilder(uint32_t root)
	{
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t Dockspace::dockWindow(CString name, uint32_t id)
	{
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return 0;
	}

	uint32_t Dockspace::splitNode(uint32_t id, int32_t dir, float ratio, uint32_t * other)
	{
		return splitNode(id, dir, ratio, NULL, other);
	}

	uint32_t Dockspace::splitNode(uint32_t id, int32_t dir, float ratio, uint32_t * out, uint32_t * other)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, other);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}