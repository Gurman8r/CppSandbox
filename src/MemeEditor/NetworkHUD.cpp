#include <MemeEditor/NetworkHUD.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	NetworkHUD::NetworkHUD()
		: base_type("Network HUD")
	{
	}

	NetworkHUD::~NetworkHUD()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void NetworkHUD::onEvent(const IEvent * value)
	{
	}

	bool NetworkHUD::draw(bool * p_open)
	{
		if (beginDraw(p_open, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::EndMenuBar();
			}
		}
		return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}