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
	
	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, size_t C, size_t R>
	inline static Matrix<T, C, R> & InputMatrix(const char * label, Matrix<T, C, R> & value)
	{
		ImGui::Text(label);
		for (size_t y = 0; y < value.Rows; y++)
		{
			ImGui::PushItemWidth(80);
			for (size_t x = 0; x < value.Cols; x++)
			{
				const size_t i = y * value.Cols + x;
				const String l = (i > 9 ? std::to_string(i) : ("0" + std::to_string(i)));

				ImGui::DragFloat(l.c_str(), &value[i]);
				ImGui::SameLine();
			}
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}
		return value;
	}

	template <typename T, size_t N>
	inline static Vector<T, N> & InputVector(const char * label, Vector<T, N> & value)
	{
		return InputMatrix<T, N, 1>(label, value);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	mat4f & Editor::InputMat4f(const char * label, mat4f & value)
	{
		return InputMatrix<float, 4, 4>(label, value);
	}

	mat3f & Editor::InputMat3f(const char * label, mat3f & value)
	{
		return InputMatrix<float, 3, 3>(label, value);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}