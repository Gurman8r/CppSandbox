#ifndef _EDITOR_GUI_HPP_
#define _EDITOR_GUI_HPP_

#include <MemeEditor/Editor.hpp>
#include <MemeGraphics/Transform.hpp>

namespace ml
{
	class ML_EDITOR_API EditorGUI final
	{
	public:
		static void InputMat4f(CString label, mat4f & value);
		static void InputMat3f(CString label, mat3f & value);
		static void InputVec2f(CString label, vec2f & value);
		static void InputVec3f(CString label, vec3f & value);
		static void InputVec4f(CString label, vec4f & value);
	};
}

#endif // !_EDITOR_GUI_HPP_
