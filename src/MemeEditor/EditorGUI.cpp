#include <MemeEditor/EditorGUI.hpp>
#include <MemeEditor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, size_t C, size_t R>
	inline static void InputMatrix(CString label, Matrix<T, C, R> & value)
	{
		const String id = String("##") + label;
		ImGui::Text(label);
		for (size_t y = 0; y < value.Rows; y++)
		{
			ImGui::PushItemWidth(50);
			for (size_t x = 0; x < value.Cols; x++)
			{
				const size_t i = y * value.Cols + x;
				const String l = String("##") + (i > 9 ? std::to_string(i) : ("0" + std::to_string(i)));

				ImGui::DragFloat((l + id).c_str(), &value[i], 0.01f);
				ImGui::SameLine();
			}
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void EditorGUI::InputMat4f(CString label, mat4f & value)
	{
		InputMatrix<float, 4, 4>(label, value);
	}

	void EditorGUI::InputMat3f(CString label, mat3f & value)
	{
		InputMatrix<float, 3, 3>(label, value);
	}

	void EditorGUI::InputVec2f(CString label, vec2f & value)
	{
		String name(label);
		size_t i;
		if ((i = name.find("##")) != String::npos)
		{
			name = name.substr(0, i);
		}
		ImGui::PushID(label);
		ImGui::PushItemWidth(64);
		ImGui::DragFloat("##X", &value[0], 0.1f, 0, 0, "X:%.3f"); ImGui::SameLine();
		ImGui::DragFloat("##Y", &value[1], 0.1f, 0, 0, "Y:%.3f"); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::Text(name.c_str());
		ImGui::PopID();
	}

	void EditorGUI::InputVec3f(CString label, vec3f & value)
	{
		String name(label);
		size_t i;
		if ((i = name.find("##")) != String::npos)
		{
			name = name.substr(0, i);
		}
		ImGui::PushID(label);
		ImGui::PushItemWidth(64);
		ImGui::DragFloat("##X", &value[0], 0.1f, 0, 0, "X:%.3f"); ImGui::SameLine();
		ImGui::DragFloat("##Y", &value[1], 0.1f, 0, 0, "Y:%.3f"); ImGui::SameLine();
		ImGui::DragFloat("##Z", &value[2], 0.1f, 0, 0, "Z:%.3f"); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::Text(name.c_str());
		ImGui::PopID();
	}

	void EditorGUI::InputVec4f(CString label, vec4f & value)
	{
		String name(label);
		size_t i;
		if ((i = name.find("##")) != String::npos)
		{
			name = name.substr(0, i);
		}
		ImGui::PushID(label);
		ImGui::PushItemWidth(64);
		ImGui::DragFloat("##X", &value[0], 0.1f, 0, 0, "X:%.3f"); ImGui::SameLine();
		ImGui::DragFloat("##Y", &value[1], 0.1f, 0, 0, "Y:%.3f"); ImGui::SameLine();
		ImGui::DragFloat("##Z", &value[2], 0.1f, 0, 0, "Z:%.3f"); ImGui::SameLine();
		ImGui::DragFloat("##W", &value[2], 0.1f, 0, 0, "W:%.3f"); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::Text(name.c_str());
		ImGui::PopID();
	}

	void EditorGUI::DrawLabel(CString label)
	{
		CString label_end;
		if ((label_end = ImGui::FindRenderedTextEnd(label)) != label)
		{
			ImGui::SameLine(0, GImGui->Style.ItemInnerSpacing.x);
			ImGui::TextEx(label, label_end);
		}
	}

	bool EditorGUI::EditVec3f(CString label, vec3f & value, float speed, float min, float max)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::BeginGroup();
			ImGui::PushID(label);
			{
				ImGui::PushItemWidth(64);
				if (ImGui::DragFloat("##X", &value[0], speed, min, max, "X:%.3f")) 
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &value[1], speed, min, max, "Y:%.3f")) 
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Z", &value[2], speed, min, max, "Z:%.3f")) 
				{
					changed = true;
				}
				ImGui::PopItemWidth();

				DrawLabel(label);
			}
			ImGui::PopID();
			ImGui::EndGroup();
			return changed;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}