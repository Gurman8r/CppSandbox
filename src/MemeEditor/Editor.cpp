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

	void Editor::ShowHelpMarker(ml::CString desc) const
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
	inline static Matrix<T, C, R> & InputMatrix(ml::CString label, Matrix<T, C, R> & value)
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

				ImGui::DragFloat((l + id).c_str(), &value[i]);
				ImGui::SameLine();
			}
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Transform & Editor::InputTransform(ml::CString label, Transform & value) const
	{
		mat4f temp = value;
		return (value = InputMat4f(label, temp));
	}

	mat4f & Editor::InputMat4f(ml::CString label, mat4f & value) const
	{
		return InputMatrix<float, 4, 4>(label, value);
	}

	mat3f & Editor::InputMat3f(ml::CString label, mat3f & value) const
	{
		return InputMatrix<float, 3, 3>(label, value);
	}

	vec2f & Editor::InputVec2f(ml::CString label, vec2f & value) const
	{
		const String id = String("##") + label;

		ImGui::PushItemWidth(80);
		ImGui::Text(label);
		ImGui::DragFloat((String("x") + id).c_str(), &value[0]); ImGui::SameLine();
		ImGui::DragFloat((String("y") + id).c_str(), &value[1]); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::NewLine();
		return value;
	}

	vec3f & Editor::InputVec3f(ml::CString label, vec3f & value) const
	{
		const String id = String("##") + label;

		ImGui::PushItemWidth(80);
		ImGui::Text(label);
		ImGui::DragFloat((String("x") + id).c_str(), &value[0]); ImGui::SameLine();
		ImGui::DragFloat((String("y") + id).c_str(), &value[1]); ImGui::SameLine();
		ImGui::DragFloat((String("z") + id).c_str(), &value[2]); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::NewLine();
		return value;
	}

	vec4f & Editor::InputVec4f(ml::CString label, vec4f & value) const
	{
		const String id = String("##") + label;

		ImGui::PushItemWidth(80);
		ImGui::Text(label);
		ImGui::DragFloat((String("x") + id).c_str(), &value[0]); ImGui::SameLine();
		ImGui::DragFloat((String("y") + id).c_str(), &value[1]); ImGui::SameLine();
		ImGui::DragFloat((String("z") + id).c_str(), &value[2]); ImGui::SameLine();
		ImGui::DragFloat((String("w") + id).c_str(), &value[3]); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::NewLine();
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}