#include <MemeEditor/Dockspace.hpp>
#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Dockspace::Dockspace()
		: GUI_Window("Dockspace")
		, m_border	(0.0f)
		, m_padding	(vec2f::Zero)
		, m_rounding(0.0f)
		, m_size	(vec2f::Zero)
		, m_bgAlpha	(1.0f)
	{
	}

	Dockspace::~Dockspace() {}

	/* * * * * * * * * * * * * * * * * * * * */

	void Dockspace::onEvent(const IEvent * value)
	{
	}

	bool Dockspace::drawGui(bool * p_open)
	{
		return beginDraw(p_open, 
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

	bool Dockspace::beginDraw(bool * p_open, int32_t flags)
	{
		if (goodCheck(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
		{
			// Bounds
			const ImGuiViewport * viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			// Style
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, m_rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, m_border);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { m_padding[0], m_padding[1] });
			ImGui::SetNextWindowBgAlpha(m_bgAlpha);

			// Begi
			if (GUI_Window::beginDraw(p_open, flags))
			{
				ImGui::PopStyleVar(3);
			}
		}
		return good();
	}

	bool Dockspace::endDraw()
	{
		if (good())
		{
			ImGui::DockSpace(
				getID(),
				{ m_size[0], m_size[1] }, 
				ImGuiDockNodeFlags_PassthruDockspace
			);
		}
		return GUI_Window::endDraw();
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