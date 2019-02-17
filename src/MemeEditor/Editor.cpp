#include <MemeEditor/Editor.hpp>
#include <MemeCore/Debug.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_ml.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Editor::Editor()
	{
	}

	Editor::~Editor()
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	void Editor::ShowHelpMarker(CString desc) const
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

	void Editor::ShowFramerate() const
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, size_t C, size_t R>
	inline static Matrix<T, C, R> & InputMatrix(CString label, Matrix<T, C, R> & value)
	{
		const String id = String("##") + label;

		ImGui::Text(label);
		for (size_t y = 0; y < value.Rows; y++)
		{
			ImGui::PushItemWidth(80);
			for (size_t x = 0; x < value.Cols; x++)
			{
				const size_t i = y * value.Cols + x;
				const String l = (i > 9 ? std::to_string(i) : ("0" + std::to_string(i)));

				ImGui::DragFloat((l + id).c_str(), &value[i], 0.01f);
				ImGui::SameLine();
			}
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Transform & Editor::InputTransform(CString label, Transform & value) const
	{
		mat4f temp = value;
		return (value = InputMat4f(label, temp));
	}

	mat4f & Editor::InputMat4f(CString label, mat4f & value) const
	{
		return InputMatrix<float, 4, 4>(label, value);
	}

	mat3f & Editor::InputMat3f(CString label, mat3f & value) const
	{
		return InputMatrix<float, 3, 3>(label, value);
	}

	vec2f & Editor::InputVec2f(CString label, vec2f & value) const
	{
		const String id = String("##") + label;

		ImGui::PushItemWidth(64);
		ImGui::DragFloat((id + "##X").c_str(), &value[0], 1, 0, 0, "X:%.3f"); ImGui::SameLine();
		ImGui::DragFloat((id + "##Y").c_str(), &value[1], 1, 0, 0, "Y:%.3f"); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::Text(label);
		return value;
	}

	vec3f & Editor::InputVec3f(CString label, vec3f & value) const
	{
		const String id = String("##") + label;

		ImGui::PushItemWidth(64);
		ImGui::DragFloat((id + "##X").c_str(), &value[0], 1, 0, 0, "X:%.3f"); ImGui::SameLine();
		ImGui::DragFloat((id + "##Y").c_str(), &value[1], 1, 0, 0, "Y:%.3f"); ImGui::SameLine();
		ImGui::DragFloat((id + "##Z").c_str(), &value[2], 1, 0, 0, "Z:%.3f"); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::Text(label);
		return value;
	}

	vec4f & Editor::InputVec4f(CString label, vec4f & value) const
	{
		const String id = String("##") + label;

		ImGui::PushItemWidth(64);
		ImGui::DragFloat((id + "##X").c_str(), &value[0], 1, 0, 0, "X:%.3f"); ImGui::SameLine();
		ImGui::DragFloat((id + "##Y").c_str(), &value[1], 1, 0, 0, "Y:%.3f"); ImGui::SameLine();
		ImGui::DragFloat((id + "##Z").c_str(), &value[2], 1, 0, 0, "Z:%.3f"); ImGui::SameLine();
		ImGui::DragFloat((id + "##W").c_str(), &value[2], 1, 0, 0, "W:%.3f"); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::Text(label);
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}