#include <MemeEditor/Editor.hpp>
#include <MemeCore/Debug.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	Editor::Editor()
	{
	}

	Editor::~Editor()
	{
	}
	
	void Editor::ShowHelpMarker(const char * desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
	Transform & Editor::InputTransform(const char * label, Transform & value)
	{
		mat4f temp = value;
		ImGui::Text(label);
		for (size_t y = 0; y < temp.Rows; y++)
		{
			ImGui::PushItemWidth(80);
			for (size_t x = 0; x < temp.Cols; x++)
			{
				const size_t i = y * temp.Cols + x;
				const String l = (i > 9 ? std::to_string(i) : ("0" + std::to_string(i)));

				ImGui::DragFloat(l.c_str(), &temp[i]);
				ImGui::SameLine();
			}
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}
		return (value = temp);
	}
}