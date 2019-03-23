#ifndef _ML_EDITOR_GUI_HPP_
#define _ML_EDITOR_GUI_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeGraphics/Transform.hpp>

#define ML_EditorGUI ml::EditorGUI::getInstance()

namespace ml
{
	class ML_EDITOR_API EditorGUI final
		: public ISingleton<EditorGUI>
	{
		friend class ISingleton<EditorGUI>;

	private:
		EditorGUI() {}
		~EditorGUI() {}

	public:
		static void InputMat4f(CString label, mat4f & value);
		static void InputMat3f(CString label, mat3f & value);
		static void InputVec2f(CString label, vec2f & value);
		static void InputVec3f(CString label, vec3f & value);
		static void InputVec4f(CString label, vec4f & value);

		static void DrawLabel(CString label);
		static bool EditVec3f(CString label, vec3f & value, float speed = 1.f, float min = 0.f, float max = 0.f);
	};
}

#endif // !_ML_EDITOR_GUI_HPP_