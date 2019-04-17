#include <MemeEditor/GUI.hpp>
#include <MemeEditor/ImGui.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemeGraphics/Shader.hpp>

namespace ml
{
	template <
		typename T, 
		size_t C, 
		size_t R
	> inline static bool EditMatrix(CString label, Matrix<T, C, R> & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::PushID(label);
			{
				ImGui::BeginGroup();
				ImGui::PushItemWidth(64);
				for (size_t y = 0; y < value.Rows; y++)
				{
					for (size_t x = 0; x < value.Cols; x++)
					{
						const size_t i = (y * value.Cols + x);
						const String l = String("##{0}").format(i);

						if (ImGui::DragFloat(l.c_str(), &value[i], speed))
						{
							changed = true;
						}
						ImGui::SameLine();
					}
					ImGui::NewLine();
				}
				ImGui::PopItemWidth();
				ImGui::EndGroup();
			}
			ImGui::PopID();
			return changed;
		}
		return false;
	}
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI::DrawLabel(CString label)
	{
		if (label)
		{
			CString label_end;
			if ((label_end = ImGui::FindRenderedTextEnd(label)) != label)
			{
				ImGui::SameLine(0, GImGui->Style.ItemInnerSpacing.x);
				ImGui::TextEx(label, label_end);
				return true;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI::EditVec2f(CString label, vec2f & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::BeginGroup();
			ImGui::PushID(label);
			{
				ImGui::PushItemWidth(64);
				if (ImGui::DragFloat("##X", &value[0], speed, 0.f, 0.f, "X:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &value[1], speed, 0.f, 0.f, "Y:%.3f"))
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

	bool GUI::EditVec3f(CString label, vec3f & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::BeginGroup();
			ImGui::PushID(label);
			{
				ImGui::PushItemWidth(64);
				if (ImGui::DragFloat("##X", &value[0], speed, 0.f, 0.f, "X:%.3f")) 
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &value[1], speed, 0.f, 0.f, "Y:%.3f")) 
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Z", &value[2], speed, 0.f, 0.f, "Z:%.3f")) 
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

	bool GUI::EditVec4f(CString label, vec4f & value, float speed)
	{
		if (!ImGui::GetCurrentWindow()->SkipItems)
		{
			bool changed = false;
			ImGui::BeginGroup();
			ImGui::PushID(label);
			{
				ImGui::PushItemWidth(64);
				if (ImGui::DragFloat("##X", &value[0], speed, 0.f, 0.f, "X:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &value[1], speed, 0.f, 0.f, "Y:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Z", &value[2], speed, 0.f, 0.f, "Z:%.3f"))
				{
					changed = true;
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##W", &value[3], speed, 0.f, 0.f, "W:%.3f"))
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

	bool GUI::EditMat3f(CString label, mat3f & value, float speed)
	{
		return EditMatrix<float, 3, 3>(label, value, speed);
	}

	bool GUI::EditMat4f(CString label, mat4f & value, float speed)
	{
		return EditMatrix<float, 4, 4>(label, value, speed);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI::EditUniform(CString label, Uniform * value)
	{
		if (value)
		{
			static CString u_types[] = {
				"None",
				"Int",
				"Float",
				"Vec2",
				"Vec3",
				"Vec4",
				"Mat3",
				"Mat4",
				"Tex2D",
			};

			ImGui::PushID(value->name.c_str());
			{
				// Type
				/* * * * * * * * * * * * * * * * * * * * */
				ImGui::Combo("Type", &value->type, u_types, IM_ARRAYSIZE(u_types));

				// Data
				/* * * * * * * * * * * * * * * * * * * * */
				switch (value->type)
				{
				case Uniform::Int:
				{
					static int32_t temp;
					ImGui::DragInt("Value", &temp);
				}
				break;
				case Uniform::Float:
				{
					static float temp;
					ImGui::DragFloat("Value", &temp, 0.1f);
				}
				break;
				case Uniform::Vec2:
				{
					static vec2f temp;
					EditVec2f("Value", temp);
				}
				break;
				case Uniform::Vec3:
				{
					static vec3f temp;
					EditVec3f("Value", temp);
				}
				break;
				case Uniform::Vec4:
				{
					static vec4f temp;
					EditVec4f("Value", temp);
				}
				break;
				case Uniform::Mat3:
				{
					static mat3f temp;
					EditMat3f("Value", temp);
				}
				break;
				case Uniform::Mat4:
				{
					static mat4f temp;
					EditMat4f("Value", temp);
				}
				break;
				case Uniform::Tex2D:
				{
					auto vector_getter = [](void* vec, int idx, const char** out_text)
					{
						auto& vector = *static_cast<List<String>*>(vec);
						if (idx < 0 || idx >= static_cast<int32_t>(vector.size()))
						{
							return false;
						}
						*out_text = vector.at(idx).c_str();
						return true;
					};

					List<String> names;
					for (auto pair : ML_Res.textures)
					{
						names.push_back(pair.first);
					}

					static int32_t index = 0;

					ImGui::Combo(
						"Texture",
						&index,
						vector_getter,
						static_cast<void *>(&names),
						(int32_t)names.size());
				}
				break;
				}
			}
			ImGui::PopID();
		}
		return (bool)(value);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool GUI::Begin(CString label, bool * p_open, int32_t flags)
	{
		return ImGui::Begin(label, p_open, flags);
	}

	void GUI::End()
	{
		return ImGui::End();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}