#include <MemeEditor/EditorApplication.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeCore/EventSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	EditorApplication::EditorApplication()
		: Application()
	{
		ML_EventSystem.addListener(EditorEvent::EV_Gui, this);
	}

	EditorApplication::~EditorApplication() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void EditorApplication::onEvent(const IEvent * value)
	{
		Application::onEvent(value);

		switch (value->eventID())
		{
		case EditorEvent::EV_Gui: 
			ImGui_ML_NewFrame();
			ImGui::NewFrame();
			{
				onGui(value->as<GuiEvent>());
			}
			ImGui::Render();
			ImGui_ML_Render(ImGui::GetDrawData());
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}